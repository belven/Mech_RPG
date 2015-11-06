
#pragma once
#include "Mech_RPG.h"
#include "BaseAIController.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "AI/Navigation/NavigationSystem.h"

ABaseAIController::ABaseAIController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	//: Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowingComponent"))) {
	objectCollision.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);
	objectCollision.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);
}

void ABaseAIController::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (GetOwner() && GetOwner()->GetDemandedController() == NULL) {
		if (GetOwner()->IsDead()) {
			UnPossess();
			//GetOwner()->Destroy(true);
		}
		else {
			if (!IsTargetValid(GetTarget())) {
				FindTarget();
			}

			if (IsTargetValid(GetTarget())) {
				AttackTarget(DeltaTime);
			}
		}
	}
}

void ABaseAIController::AttackTarget(float DeltaTime) {
	SetupCollision();

	GetWorld()->LineTraceSingleByObjectType(hit, GetOwner()->GetActorLocation(), target->GetActorLocation(), objectCollision, collision);

	bool targetTraced = hit.bBlockingHit && hit.GetActor() != NULL;

	// Are we targeting ourselves
	if (target == GetOwner()) {
		PerformAbility();
		FireWeapon(NULL);
	}
	// Have we traced to another character or cover
	else if (targetTraced 
		&& (UMiscLibrary::IsMechCharacter(hit.GetActor()) || UMiscLibrary::IsCover(hit.GetActor()))) {
		PerformAbility();
		FireWeapon(hit.GetActor());
	}
	// We've hit some scenery so move towards the target
	else if (GetWorld()->GetNavigationSystem()) {
		MoveToLocation(target->GetActorLocation());
	}
}


void ABaseAIController::SetupCollision() {
	collision.IgnoreComponents.Empty();

	if (GetOwner()->GetGroup() != NULL && GetOwner()->GetGroup()->HasMemebers()) {
		for (AMech_RPGCharacter* member : GetOwner()->GetGroup()->GetMembers()) {
			if (member != target) {
				collision.AddIgnoredActor(member);
			}
		}
		GetOwner()->GetGroup()->GroupMemberHit(target, GetOwner());
	}
}

void ABaseAIController::FireWeapon(AActor* hit) {
	AWeapon* weapon = GetOwner()->GetCurrentWeapon();
	float distToTarget = GetOwner()->GetDistanceTo(target);

	// Are we in weapons range
	if (weapon != NULL && distToTarget <= weapon->GetRange()) {
		if (GetOwner()->CanAttack() && weapon->CanFire()) {

			bool isCover = hit != NULL ? UMiscLibrary::IsCover(hit) : false;

			// Have we hit cover
			if (isCover) {
				float distToCover = GetOwner()->GetDistanceTo(hit);

				// Are we too far from the cover to avoid shooting it
				if (distToCover > 200) {
					weapon->Fire(Cast<ACover>(hit), GetOwner());
				}
				// Otherwise we can attack the target
				else {
					weapon->Fire(target, GetOwner());
				}
			}
			// Otherwise we've got a clear shot to the target
			else {
				weapon->Fire(target, GetOwner());
			}
		}

		SetFocus(target);
		StopMovement();
	}
	// We're out of range so move closer
	else if (GetWorld()->GetNavigationSystem()) {
		MoveToLocation(target->GetActorLocation());
	}
}

void ABaseAIController::PerformAbility() {
	if (GetOwner()->HasAbilities() 
		&& !GetOwner()->Channelling() 
		&& GetOwner()->CanCast()) {
		for (UAbility* ability : GetOwner()->GetAbilities()) {
			if (ability != NULL && !ability->OnCooldown()) {
				ability->Activate(target, target->GetActorLocation());
				GetOwner()->SetCurrentAbility(ability);
				break;
			}
		}
	}
}

void ABaseAIController::MoveToActor(AActor* target) {
	if (GetOwner()->CanMove()) {
		GetWorld()->GetNavigationSystem()->SimpleMoveToActor(this, target);
	}
}

void ABaseAIController::MoveToLocation(FVector location) {
	if (GetOwner()->CanMove()) {
		GetWorld()->GetNavigationSystem()->SimpleMoveToLocation(this, location);
	}
}

void ABaseAIController::FindTarget() {
	AWeapon* weapon = GetOwner()->GetCurrentWeapon();

	collision.IgnoreComponents.Empty();

	if (GetOwner()->GetGroup() != NULL && GetOwner()->GetGroup()->HasMemebers()) {
		for (AMech_RPGCharacter* member : GetOwner()->GetGroup()->GetMembers()) {
			if (member != target) {
				collision.AddIgnoredActor(member);
			}
		}
	}

	if (weapon != NULL && !weapon->Heals()) {
		float range = weapon->GetRange() * 1.3;
		range = range < 1700 ? 1700 : range;

		for (AMech_RPGCharacter* character : GetCharactersInRange(range)) {
			GetWorld()->LineTraceSingleByObjectType(hit, GetOwner()->GetActorLocation(), character->GetActorLocation(), objectCollision, collision);

			if (hit.bBlockingHit
				&& hit.GetActor() != NULL
				&& (hit.GetActor() == character || UMiscLibrary::IsCover(hit.GetActor()))
				&& !character->CompareGroup(GetOwner())) {
				SetTarget(character);
				break;
			}
		}
	}
	else if (weapon != NULL && GetOwner()->GetGroup() != NULL && GetOwner()->GetGroup()->HasMemebers()) {
		for (AMech_RPGCharacter* character : GetOwner()->GetGroup()->GetMembers()) {
			if (UMiscLibrary::IsCharacterAlive(character) && UMiscLibrary::GetMissingHealth(character) > 0) {
				SetTarget(character);
				break;
			}
		}
	}
}

TArray<AMech_RPGCharacter*> ABaseAIController::GetCharactersInRange(float range) {
	return UMiscLibrary::GetCharactersInRange(range, GetOwner());
}

AMech_RPGCharacter* ABaseAIController::GetOwner() {
	return characterOwner;
}

AMech_RPGCharacter* ABaseAIController::GetTarget() {
	return target;
}

bool ABaseAIController::IsTargetValid(AMech_RPGCharacter* inTarget) {
	if (UMiscLibrary::IsCharacterAlive(inTarget)) {
		if (GetOwner()->GetCurrentWeapon()) {
			if (GetOwner()->GetCurrentWeapon()->Heals()) {
				return inTarget->CompareGroup(GetOwner()) && UMiscLibrary::GetMissingHealth(inTarget) > 0;
			}
			else {
				return !inTarget->CompareGroup(GetOwner());
			}
		}
	}
	return false;
}

void ABaseAIController::SetOwner(AMech_RPGCharacter* newVal) {
	characterOwner = newVal;
}

void ABaseAIController::SetTarget(AMech_RPGCharacter* newVal) {
	target = newVal;
}

void ABaseAIController::GroupMemberDamaged(AMech_RPGCharacter* attacker, AMech_RPGCharacter* damagedMember) {
	if (!GetTarget() || !IsTargetValid(GetTarget())) {
		if (IsTargetValid(attacker)) {
			SetTarget(attacker);
		}
		else if (IsTargetValid(damagedMember)) {
			SetTarget(damagedMember);
		}
	}
}