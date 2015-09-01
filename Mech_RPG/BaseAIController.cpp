
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
			GetOwner()->Destroy(true);
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
	/*if (GetOwner()->GetAbilities().Num() > 0) {
		if (!GetOwner()->GetAbilities()[0]->OnCooldown()) {
		GetOwner()->GetAbilities()[0]->Activate(GetOwner(), target);
		}
		}*/

	AWeapon* weapon = owner->GetCurrentWeapon();

	if (weapon) {
		float dist = FVector::Dist(owner->GetActorLocation(), target->GetActorLocation());

		if (dist <= weapon->GetRange()) {
			if (weapon->CanFire()) {
				weapon->Fire(target, GetOwner());
			}
			StopMovement();
		}
		else if (GetWorld()->GetNavigationSystem()) {
			GetWorld()->GetNavigationSystem()->SimpleMoveToLocation(this, target->GetActorLocation());
		}
	}
}

void ABaseAIController::FindTarget() {
	for (FConstPawnIterator iter = GetWorld()->GetPawnIterator(); iter; iter++) {
		APawn* pawn = iter->Get();
		if (pawn && pawn != GetOwner() && pawn->GetDistanceTo(GetOwner()) <= GetOwner()->GetAOE()->GetUnscaledSphereRadius()) {
			AMech_RPGCharacter* character = Cast<AMech_RPGCharacter>(pawn);
			if (!character->IsDead() && !character->CompareGroup(GetOwner())) {
				SetTarget(character);
				break;
			}
		}
	}
	
	/*TArray<AActor*>& tempActors = *new TArray<AActor*>();

	GetOwner()->GetAOE()->GetOverlappingActors(tempActors, AMech_RPGCharacter::StaticClass());

	for (AActor* temp : tempActors) {
		AMech_RPGCharacter* sp = Cast<AMech_RPGCharacter>(temp);

		if (sp && sp != GetOwner()) {
			AMech_RPGCharacter* character = Cast<AMech_RPGCharacter>(sp);

			if (!character->IsDead() && !character->CompareGroup(GetOwner())) {
				SetTarget(character);
				break;
			}
		}
	}*/
}

void ABaseAIController::OnOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	//if (!IsTargetValid() && OtherActor && OtherActor != GetOwner()) {
	//	AMech_RPGCharacter* sp = Cast<AMech_RPGCharacter>(OtherActor);

	//	if (sp && sp != GetOwner()) {
	//		AMech_RPGCharacter* character = Cast<AMech_RPGCharacter>(sp);

	//		if (!character->IsDead() && !character->CompareGroup(GetOwner())) {
	//			SetTarget(character);
	//		}
	//	}
	//}
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
	return target && !target->IsDead() && !target->CompareGroup(owner);
}

void ABaseAIController::SetOwner(AMech_RPGCharacter* newVal) {
	owner = newVal;
}

void ABaseAIController::SetTarget(AMech_RPGCharacter* newVal) {
	target = newVal;
}