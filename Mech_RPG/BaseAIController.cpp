
#pragma once
#include "Mech_RPG.h"
#include "BaseAIController.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "Mech_RPGCharacter.h"
#include "AI/Navigation/NavigationSystem.h"

ABaseAIController::ABaseAIController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowingComponent"))) {

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
	AWeapon* weapon = characterOwner->GetCurrentWeapon();
	collision.IgnoreComponents.Empty();

	if (characterOwner->GetGroup() != NULL && characterOwner->GetGroup()->GetMembers().Num() > 0) {
		for (AMech_RPGCharacter* member : characterOwner->GetGroup()->GetMembers()) {
			if (member != target) {
				collision.AddIgnoredActor(member);
			}
		}
		characterOwner->GetGroup()->GroupMemberHit(target, characterOwner);
	}

	GetWorld()->LineTraceSingle(hit, characterOwner->GetActorLocation(), target->GetActorLocation(), collision, NULL);


	if (target == GetOwner() || (hit.GetActor() != NULL && IsMechCharacter(hit.GetActor()))) {
		if (GetOwner()->GetAbilities().Num() > 0) {
			for (UAbility* ability : GetOwner()->GetAbilities()) {
				if (&ability != NULL && ability != NULL &&!ability->OnCooldown()) {
					ability->Activate(target);
					break;
				}
			}
		}

		float dist = FVector::Dist(characterOwner->GetActorLocation(), target->GetActorLocation());

		if (weapon != NULL && dist <= weapon->GetRange()) {
			if (GetOwner()->CanAttack() && weapon->CanFire()) {
				weapon->Fire(target, GetOwner());
			}
			SetFocus(target);
			StopMovement();
		}
		else if (GetWorld()->GetNavigationSystem()) {
			MoveToLocation(target->GetActorLocation());
		}
	}
	else if (GetWorld()->GetNavigationSystem()) {
		MoveToLocation(target->GetActorLocation());
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

bool ABaseAIController::IsMechCharacter(AActor* character) {
	return character->GetClass()->IsChildOf(AMech_RPGCharacter::StaticClass());
}

void ABaseAIController::FindTarget() {
	AWeapon* weapon = characterOwner->GetCurrentWeapon();

	collision.IgnoreComponents.Empty();

	if (characterOwner->GetGroup() != NULL && characterOwner->GetGroup()->GetMembers().Num() > 0) {
		for (AMech_RPGCharacter* member : characterOwner->GetGroup()->GetMembers()) {
			if (member != target) {
				collision.AddIgnoredActor(member);
			}
		}
	}

	if (weapon != NULL && !weapon->Heals()) {
		float range = weapon->GetRange() * 1.3;
		range = range < 1200 ? 1200 : range;

		for (FConstPawnIterator iter = GetWorld()->GetPawnIterator(); iter; iter++) {
			APawn* pawn = iter->Get();

			GetWorld()->LineTraceSingle(hit, characterOwner->GetActorLocation(), pawn->GetActorLocation(), collision, NULL);

			if (pawn != NULL && IsMechCharacter(pawn) && pawn->GetDistanceTo(GetOwner()) <= range && hit.GetActor() == pawn) {
				AMech_RPGCharacter* character = Cast<AMech_RPGCharacter>(pawn);

				if (!character->IsDead() && !character->CompareGroup(characterOwner)) {
					SetTarget(character);
					break;
				}
			}
		}
	}
	else if (weapon != NULL && GetOwner()->GetGroup() != NULL && GetOwner()->GetGroup()->GetMembers().Num() > 0) {
		for (AMech_RPGCharacter* character : GetOwner()->GetGroup()->GetMembers()) {
			if (!character->IsDead() && character->GetHealth() < character->GetMaxHealth()) {
				SetTarget(character);
				break;
			}
		}
	}
}

AMech_RPGCharacter* ABaseAIController::GetOwner() {
	return characterOwner;
}

AMech_RPGCharacter* ABaseAIController::GetTarget() {
	return target;
}

bool ABaseAIController::IsTargetValid(AMech_RPGCharacter* inTarget) {
	if (inTarget != NULL && !inTarget->IsDead()) {
		if (GetOwner()->GetCurrentWeapon()) {
			if (GetOwner()->GetCurrentWeapon()->Heals()) {
				return inTarget->CompareGroup(characterOwner) && inTarget->GetHealth() < inTarget->GetMaxHealth();
			}
			else {
				return !inTarget->CompareGroup(characterOwner);
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