
#pragma once
#include "Mech_RPG.h"
#include "BaseAIController.h"
#include "Mech_RPGCharacter.h"
#include "AI/Navigation/NavigationSystem.h"

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
	AWeapon* weapon = owner->GetCurrentWeapon();
	collision.IgnoreComponents.Empty();

	if (owner->GetGroup() != NULL && owner->GetGroup()->GetMembers().Num() > 0) {
		for (AMech_RPGCharacter* member : owner->GetGroup()->GetMembers()) {
			if (member != target) {
				collision.AddIgnoredActor(member);
			}
		}
	}


	if (GetOwner()->GetAbilities().Num() > 0) {
		for (UAbility* ability : GetOwner()->GetAbilities()) {
			if (!ability->OnCooldown()) {
				ability->Activate(target);
			}
		}
	}

	if (weapon != NULL) {
		GetWorld()->LineTraceSingle(hit, owner->GetActorLocation(), target->GetActorLocation(), collision, NULL);

		float dist = FVector::Dist(owner->GetActorLocation(), target->GetActorLocation());

		if (dist <= weapon->GetRange() && hit.GetActor() != NULL && IsMechCharacter(hit.GetActor())) {
			if (GetOwner()->CanAttack() && weapon->CanFire()) {
				weapon->Fire(target, GetOwner());
			}
			StopMovement();
		}
		else if (GetWorld()->GetNavigationSystem()) {
			MoveToLocation(target->GetActorLocation());
		}
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("Weapon NULL"));
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
	AWeapon* weapon = owner->GetCurrentWeapon();

	if (weapon != NULL && !weapon->Heals()) {
		float range = weapon->GetRange() * 1.3;
		range = range < 1200 ? 1200 : range;

		for (FConstPawnIterator iter = GetWorld()->GetPawnIterator(); iter; iter++) {
			APawn* pawn = iter->Get();
			if (pawn != NULL && IsMechCharacter(pawn) && pawn != GetOwner() && pawn->GetDistanceTo(GetOwner()) <= range) {
				AMech_RPGCharacter* character = Cast<AMech_RPGCharacter>(pawn);

				if (!character->IsDead() && !character->CompareGroup(owner)) {
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

void ABaseAIController::OnOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {

}

void ABaseAIController::OnOverlapEnd(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {

}

AMech_RPGCharacter* ABaseAIController::GetOwner() {
	return owner;
}

AMech_RPGCharacter* ABaseAIController::GetTarget() {
	return target;
}

bool ABaseAIController::IsTargetValid(AMech_RPGCharacter* inTarget) {
	if (inTarget != NULL && !inTarget->IsDead()) {
		if (GetOwner()->GetCurrentWeapon()) {
			if (GetOwner()->GetCurrentWeapon()->Heals()) {
				return inTarget->CompareGroup(owner) && inTarget->GetHealth() < inTarget->GetMaxHealth();
			}
			else {
				return !inTarget->CompareGroup(owner);
			}
		}
	}
	return false;
}

void ABaseAIController::SetOwner(AMech_RPGCharacter* newVal) {
	owner = newVal;
}

void ABaseAIController::SetTarget(AMech_RPGCharacter* newVal) {
	target = newVal;
}

void ABaseAIController::GroupMemberDamaged(AMech_RPGCharacter* attacker, AMech_RPGCharacter* damagedMember) {
	if (!GetTarget()) {
		if (IsTargetValid(attacker)) {
			SetTarget(attacker);
		}
		else if (IsTargetValid(damagedMember)) {
			SetTarget(damagedMember);
		}
	}
}