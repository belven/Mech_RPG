// Fill out your copyright notice in the Description page of Project Settings.

#include "Mech_RPG.h"
#include "AllyAIController.h"
#include "Mech_RPGCharacter.h"
#include "Group.h"
#include "AI/Navigation/NavigationSystem.h"

void AAllyAIController::Tick(float DeltaTime) {

	if (GetOwner() && GetOwner()->GetDemandedController() == NULL) {
		if (GetOwner()->IsDead()) {
			UnPossess();
			GetOwner()->Destroy(true);
		}
		else if (GetPlayerControlledLocation() != FVector::ZeroVector) {
			if (GetOwner()->GetActorLocation() != GetPlayerControlledLocation()) {
				GetWorld()->GetNavigationSystem()->SimpleMoveToLocation(this, GetPlayerControlledLocation());
			}
			else {
				FindTargetInWeaponRage();

				if (IsTargetValid()) {
					AttackTarget(DeltaTime);
				}
			}
		}
		else {
			if (!IsTargetValid()) {
				FindTarget();
			}

			if (IsTargetValid()) {
				AttackTarget(DeltaTime);
			}
			else {
				if (GetOwner()->GetGroup()->GetPlayer()) {
					GetWorld()->GetNavigationSystem()->SimpleMoveToActor(this, GetOwner()->GetGroup()->GetPlayer());
				}
			}
		}
	}
}

void AAllyAIController::BeginPlay() {
	SetPlayerControlledLocation(FVector::ZeroVector);
}

FVector AAllyAIController::GetPlayerControlledLocation() {
	return playerControlledLocation;
}


void AAllyAIController::SetPlayerControlledLocation(FVector newVal) {
	playerControlledLocation = newVal;
}

void AAllyAIController::FindTargetInWeaponRage() {
	for (FConstPawnIterator iter = GetWorld()->GetPawnIterator(); iter; iter++) {
		APawn* pawn = iter->Get();
		if (pawn && pawn != GetOwner() && pawn->GetDistanceTo(GetOwner()) <= GetOwner()->GetCurrentWeapon()->GetRange()) {
			AMech_RPGCharacter* character = Cast<AMech_RPGCharacter>(pawn);
			if (!character->IsDead() && !character->CompareGroup(GetOwner())) {
				SetTarget(character);
				break;
			}
		}
	}
}