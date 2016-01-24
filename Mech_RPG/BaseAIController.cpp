
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

	if (GetOwner() && GetOwner()->GetDemandedController() == nullptr) {
		if (GetOwner()->IsDead()) {
			//UnPossess();
			//GetOwner()->Destroy(true);
		}
		else {
			if (!IsTargetValid(GetTarget())) {
				GetOwner()->OnStopFiring.Broadcast();
				FindTarget();
			}

			if (IsTargetValid(GetTarget())) {
				AttackTarget(DeltaTime);
			}
			else {
				GetOwner()->OnStopFiring.Broadcast();
			}
		}
	}
}

void ABaseAIController::AttackTarget(float DeltaTime) {
	// Are we targeting ourselves
	if (target == GetOwner()) {
		PerformAbility();
		FireWeapon(nullptr);
		GetOwner()->LookAt(target);
	}
	// Have we traced to another character or cover
	else if (UMiscLibrary::CanSee(GetWorld(), GetOwner()->GetActorLocation(), target->GetActorLocation())) {
		PerformAbility();
		FireWeapon(target);
		GetOwner()->LookAt(target);
	}
	// We've hit some scenery so move towards the target
	else if (GetWorld()->GetNavigationSystem()) {
		MoveToLocation(target->GetActorLocation());
	}
}


void ABaseAIController::SetupCollision() {
	collision.IgnoreComponents.Empty();

	if (GetOwner()->GetGroup() != nullptr && GetOwner()->GetGroup()->HasMemebers()) {
		for (AMech_RPGCharacter* member : GetOwner()->GetGroup()->GetMembers()) {
			if (member != target) {
				collision.AddIgnoredActor(member);
			}
		}
	}
}

void ABaseAIController::FireWeapon(AActor* hit) {
	AWeapon* weapon = GetOwner()->GetCurrentWeapon();
	float distToTarget = GetOwner()->GetDistanceTo(target);

	// Are we in weapons range
	if (weapon != nullptr && distToTarget <= weapon->GetRange()) {
		if (GetOwner()->CanAttack() && weapon->CanFire()) {

			bool isCover = hit != nullptr ? UMiscLibrary::IsCover(hit) : false;

			// Have we hit cover
			if (isCover) {
				float distToCover = GetOwner()->GetDistanceTo(hit);

				// Are we too far from the cover to avoid shooting it
				if (distToCover > 200) {
					weapon->Fire(Cast<ACover>(hit));
				}
				// Otherwise we can attack the target
				else {
					weapon->Fire(target);
				}
			}
			// Otherwise we've got a clear shot to the target
			else {
				weapon->Fire(target);
				GetOwner()->GetGroup()->GroupMemberHit(GetOwner(), target);
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
			if (ability != nullptr && !ability->OnCooldown()) {
				ability->Activate(target, target->GetActorLocation());
				GetOwner()->SetCurrentAbility(ability);
				break;
			}
		}
	}
}

void ABaseAIController::MoveToActor(AActor* target) {
	if (GetOwner()->CanMove()) {
		GetOwner()->OnStopFiring.Broadcast();
		GetWorld()->GetNavigationSystem()->SimpleMoveToActor(this, target);
	}
}

void ABaseAIController::MoveToLocation(FVector location) {
	if (GetOwner()->CanMove()) {
		GetOwner()->OnStopFiring.Broadcast();
		GetWorld()->GetNavigationSystem()->SimpleMoveToLocation(this, location);
	}
}

void ABaseAIController::FindTarget() {
	AWeapon* weapon = GetOwner()->GetCurrentWeapon();
	SetupCollision();

	if (weapon != nullptr && !weapon->Heals()) {
		float range = weapon->GetRange();

		for (AMech_RPGCharacter* character : GetCharactersInRange(range)) {
			//TArray<FHitResult> results;
			//GetWorld()->LineTraceMultiByChannel(results, GetOwner()->GetActorLocation(), character->GetActorLocation(), ECollisionChannel::ECC_WorldStatic);
			//bool canSee = (results.Num() == 0);

			if (IsTargetValid(character) && UMiscLibrary::CanSee(GetWorld(), GetOwner()->GetActorLocation(), character->GetActorLocation())) {
				if (character->GetGroup() != nullptr && character->GetGroup()->HasMemebers()) {
					SetTarget(character->GetGroup()->GetRandomMember());
				}
				else {
					SetTarget(character);
				}
				break;
			}
		}
	}
	else if (weapon != nullptr && GetOwner()->GetGroup() != nullptr && GetOwner()->GetGroup()->HasMemebers()) {
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