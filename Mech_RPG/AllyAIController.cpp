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
	if (GetAIOwner() && !GetAIOwner()->IsDead() && GetAIOwner()->GetDemandedController() == NULL) {
		if (GetPlayerControlledLocation() != FVector::ZeroVector) {
			float dist = FVector::Dist(GetPlayerControlledLocation(), GetAIOwner()->GetActorLocation());
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

			if (!IsTargetValid(GetTarget(), EAffectedTeam::Enemy) && !IsTargetValid(GetTarget(), EAffectedTeam::Ally)) {
				AMech_RPGCharacter* player = GetAIOwner()->GetGroup()->GetPlayer();
				if (ShouldMoveToPlayer(player)) {
					MoveToActor(player);
				}
				else {
					StopMovement();
				}
			}
		}
	}
}

bool AAllyAIController::ShouldMoveToPlayer(AMech_RPGCharacter* player)
{
	if (player != nullptr) {
		int32 indexOfByKey = GetAIOwner()->GetGroup()->GetMembers().IndexOfByKey(GetAIOwner());

		// This will try and spread the bots out a bit based on how many there are
		float distance = player->GetInCombat() ? GetAIOwner()->GetCurrentWeapon()->GetRange() : 300.0F + (indexOfByKey * 60);

		return player->GetDistanceTo(GetAIOwner()) > distance;
	}
	return false;
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

