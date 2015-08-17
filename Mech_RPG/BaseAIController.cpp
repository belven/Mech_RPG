
#pragma once
#include "Mech_RPG.h"
#include "BaseAIController.h"
#include "Mech_RPGCharacter.h"
#include "AI/Navigation/NavigationSystem.h"


void ABaseAIController::Possess(APawn* InPawn) {
	if (InPawn && InPawn->GetClass()->IsChildOf(AMech_RPGCharacter::StaticClass())) {
		SetOwner(Cast<AMech_RPGCharacter>(InPawn));
	}
}

void ABaseAIController::Tick(float DeltaTime) {
	//Super::Tick(DeltaTime);

	if (GetOwner()){
		if (!IsTargetValid()){
			FindTarget();
		}

		if (IsTargetValid()){
			AttackTarget(DeltaTime);
		}
	}
}

void ABaseAIController::AttackTarget(float DeltaTime){
	AWeapon* weapon = owner->GetCurrentWeapon();

	if (weapon){
		bool targetInRange = false;

		float dist = FVector::Dist(owner->GetActorLocation(), target->GetActorLocation());

		if (dist <= weapon->GetRange()) {
			if (weapon->CanFire(DeltaTime)){
				target->Hit(owner, weapon->GetDamage());
			}

			targetInRange = true;
		}

		if (!targetInRange  && GetWorld()->GetNavigationSystem())
		{
			//MoveToActor(GetTarget());
			GetWorld()->GetNavigationSystem()->SimpleMoveToLocation(this, target->GetActorLocation());
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

			if (!character->IsDead() && character->GetGroup()->GetID() != owner->GetGroup()->GetID()){
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
		return target && !target->IsDead() && target->GetGroup()->GetID() != owner->GetGroup()->GetID();
}

void ABaseAIController::MoveToTarget(){

}

void ABaseAIController::SetOwner(AMech_RPGCharacter* newVal){
	owner = newVal;
}

void ABaseAIController::SetTarget(AMech_RPGCharacter* newVal){
	target = newVal;
}