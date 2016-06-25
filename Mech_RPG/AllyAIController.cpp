#pragma once
#include "Mech_RPG.h"
#include "AllyAIController.h"
#include "Characters/Mech_RPGCharacter.h"
#include "Group.h"
#include "Weapons.h"
#include "AI/Navigation/NavigationSystem.h"

AAllyAIController::AAllyAIController() : Super() {
	//SetActorTickEnabled(true);
	//PrimaryActorTick.bCanEverTick = true;
	//PrimaryActorTick.bStartWithTickEnabled = true;
}

void AAllyAIController::Tick(float DeltaTime) {
	//UE_LOG(LogTemp, Log, TEXT("AAllyAIController Tick"));
	if (GetOwner() && !GetOwner()->IsDead() && GetOwner()->GetDemandedController() == NULL) {
		if (GetPlayerControlledLocation() != FVector::ZeroVector) {
			float dist = FVector::Dist(GetPlayerControlledLocation(), GetOwner()->GetActorLocation());
			if (dist > 1800.0F) {
				SetPlayerControlledLocation(FVector::ZeroVector);
			}
			else if (dist > 150.0F) {
				MoveToLocation(GetPlayerControlledLocation());
			}
			else {
				SetPlayerControlledLocation(FVector::ZeroVector);
			}
		}
		else {
			Super::Tick(DeltaTime);

			if (!IsTargetValid(GetTarget(), false) && !IsTargetValid(GetTarget(), true)) {
				AMech_RPGCharacter* player = GetOwner()->GetGroup()->GetPlayer();
				if (player != NULL && player->GetDistanceTo(GetOwner()) > (300.0F + (GetOwner()->GetGroup()->GetMembers().IndexOfByKey(GetOwner()) * 40))) {
					MoveToActor(player);
				}
				else {
					StopMovement();
				}
			}
		}
	}
}

void AAllyAIController::BeginPlay() {
	Super::BeginPlay();
	SetPlayerControlledLocation(FVector::ZeroVector);
}

FVector AAllyAIController::GetPlayerControlledLocation() {
	return playerControlledLocation;
}

void AAllyAIController::SetPlayerControlledLocation(FVector newVal) {
	playerControlledLocation = newVal;
}

