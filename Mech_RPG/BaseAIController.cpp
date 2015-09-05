
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
			if (!IsTargetValid()) {
				FindTarget();
			}

			if (IsTargetValid()) {
				AttackTarget(DeltaTime);
			}
		}
	}
}

void ABaseAIController::AttackTarget(float DeltaTime) {
	AWeapon* weapon = owner->GetCurrentWeapon();
	collision.AddIgnoredActor(GetOwner());

	/*if (!collisionSet) {
		for (AMech_RPGCharacter* member : owner->GetGroup()->GetMembers()) {
			collision.AddIgnoredActor(member);
		}
		collisionSet = true;
	}
	*/

	if (GetOwner()->startingGroupID == 0 && GetOwner()->GetAbilities().Num() > 0) {
		if (!GetOwner()->GetAbilities()[0]->OnCooldown()) {
			GetOwner()->GetAbilities()[0]->Activate(target);
		}
	}

	if (weapon != NULL) {
		GetWorld()->LineTraceSingle(hit, owner->GetActorLocation(), target->GetActorLocation(), collision, NULL);

		float dist = FVector::Dist(owner->GetActorLocation(), target->GetActorLocation());

		if (dist <= weapon->GetRange() && hit.GetActor() == target) {
			if (weapon->CanFire()) {
				weapon->Fire(target, GetOwner());
			}
			StopMovement();
		}
		else if (GetWorld()->GetNavigationSystem()) {
			GetWorld()->GetNavigationSystem()->SimpleMoveToLocation(this, target->GetActorLocation());
		}
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("Weapon NULL"));
	}
}

void ABaseAIController::FindTarget() {
	AWeapon* weapon = owner->GetCurrentWeapon();
	if (weapon != NULL && !weapon->Heals()) {
		for (FConstPawnIterator iter = GetWorld()->GetPawnIterator(); iter; iter++) {
			APawn* pawn = iter->Get();
			if (pawn != NULL && pawn != GetOwner() && pawn->GetDistanceTo(GetOwner()) <= weapon->GetRange() * 1.3) {
				AMech_RPGCharacter* character = Cast<AMech_RPGCharacter>(pawn);

				if (!character->IsDead() && !character->CompareGroup(owner)) {
					SetTarget(character);
					break;
				}
			}
		}
	}
	else if (weapon != NULL) {
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

bool ABaseAIController::IsTargetValid() {
	if (target != NULL && !target->IsDead()) {
		if (GetOwner()->GetCurrentWeapon()) {
			if (GetOwner()->GetCurrentWeapon()->Heals()) {
				return target->CompareGroup(owner) && target->GetHealth() < target->GetMaxHealth();
			}
			else {
				return !target->CompareGroup(owner);
			}
		}
		return false;
	}
	return false;
}

void ABaseAIController::SetOwner(AMech_RPGCharacter* newVal) {
	owner = newVal;
}

void ABaseAIController::SetTarget(AMech_RPGCharacter* newVal) {
	target = newVal;
}