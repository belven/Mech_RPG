
#pragma once
#include "Mech_RPG.h"
#include "BaseAIController.h"
#include "Mech_RPGCharacter.h"
#include "AI/Navigation/NavigationSystem.h"

void ABaseAIController::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (GetOwner() && GetOwner()->GetDemandedController() == NULL){
		if (GetOwner()->IsDead()){
			UnPossess();
			GetOwner()->Destroy(true);
		}
		else {
			if (!IsTargetValid()){
				FindTarget();
			}

			if (IsTargetValid()){
				AttackTarget(DeltaTime);
			}
		}
	}
}

void ABaseAIController::AttackTarget(float DeltaTime){
	AWeapon* weapon = owner->GetCurrentWeapon();

	/*if (GetOwner()->GetAbilities().Num() > 0) {
		if (!GetOwner()->GetAbilities()[0]->OnCooldown()) {
			GetOwner()->GetAbilities()[0]->Activate(GetOwner(), target);
		}
	}*/

	if (weapon){
		bool targetInRange = false;

		float dist = FVector::Dist(owner->GetActorLocation(), target->GetActorLocation());

		if (dist <= weapon->GetRange()) {
			if (weapon->CanFire()){
				weapon->Fire(target, GetOwner());
			}

			targetInRange = true;
		}

		if (!targetInRange && GetWorld()->GetNavigationSystem())
		{
			GetWorld()->GetNavigationSystem()->SimpleMoveToActor(this, target);
		}
		else {
			StopMovement();
		}
	}
}

void ABaseAIController::FindTarget(){
	for (FConstPawnIterator iter = GetOwner()->GetWorld()->GetPawnIterator(); iter; iter++){
		APawn* pawn = iter->Get();
		if (iter->IsValid()
			&& pawn != GetOwner()
			&& pawn->GetClass()->IsChildOf(AMech_RPGCharacter::StaticClass())) {
			AMech_RPGCharacter* character = Cast<AMech_RPGCharacter>(pawn);

			if (!character->IsDead() && !character->CompareGroup(owner)){
				SetTarget(character);
				break;
			}
		}
	}
}

AMech_RPGCharacter* ABaseAIController::GetOwner(){
	return owner;
}

AMech_RPGCharacter* ABaseAIController::GetTarget(){
	return target;
}

bool ABaseAIController::IsTargetValid(){
	return target && !target->IsDead() && !target->CompareGroup(owner);
}

void ABaseAIController::SetOwner(AMech_RPGCharacter* newVal){
	owner = newVal;
}

void ABaseAIController::SetTarget(AMech_RPGCharacter* newVal){
	target = newVal;
}