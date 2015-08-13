
#pragma once
#include "Mech_RPG.h"
#include "BaseAIController.h"
#include "Mech_RPGCharacter.h"


void ABaseAIController::Possess(APawn* InPawn) {
	if (InPawn && InPawn->GetActorClass()->IsChildOf(AMech_RPGCharacter::StaticClass())) {
		SetOwner(Cast<AMech_RPGCharacter>(InPawn));
	}
}

void ABaseAIController::Tick(float DeltaTime) {
	if (GetOwner()){
		FindTarget();

		if (IsTargetValid()){
			AttackTarget(DeltaTime);
		}
	}
}

void ABaseAIController::AttackTarget(float DeltaTime){
	if (owner && owner->GetWeapons().Num() > 0){
		bool targetInRange = false;

		for (AWeapon* weapon : owner->GetWeapons()) {
			float dist = FVector::Dist(owner->GetActorLocation(), target->GetActorLocation());

			if (dist <= weapon->GetRange()) {
				if (weapon->CanFire(DeltaTime)){
					target->Hit(owner, weapon->GetDamage(), NULL);
				}

				targetInRange = true;
			}
		}

		if (!targetInRange && GetWorld()->GetNavigationSystem())
		{
			SetFocus(target);
			MoveToActor(target);
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
			&& pawn->GetActorClass()->IsChildOf(AMech_RPGCharacter::StaticClass())) {
			AMech_RPGCharacter* character = Cast<AMech_RPGCharacter>(pawn);

			if (!character->IsDead()){
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
	return target && !target->IsDead();
}

void ABaseAIController::MoveToTarget(){

}

void ABaseAIController::SetOwner(AMech_RPGCharacter* newVal){
	owner = newVal;
}

void ABaseAIController::SetTarget(AMech_RPGCharacter* newVal){
	target = newVal;
}