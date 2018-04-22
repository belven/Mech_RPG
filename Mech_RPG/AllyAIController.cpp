#pragma once
#include "Mech_RPG.h"
#include "AllyAIController.h"
#include "Characters/Mech_RPGCharacter.h"
#include "Group.h"
#include "Weapons.h"
#include "AI/Navigation/NavigationSystem.h"

AAllyAIController::AAllyAIController() : ABaseAIController()
{
}

void AAllyAIController::Tick(float DeltaTime)
{
	if (UMiscLibrary::IsCharacterAlive(GetAIOwner()) && GetAIOwner()->GetDemandedController() == nullptr)
	{
		if (GetPlayerControlledLocation() != FVector::ZeroVector)
		{
			float dist = FVector::Dist(GetPlayerControlledLocation(), GetAIOwner()->GetActorLocation());
			if (dist > 1800.0F)
			{
				SetPlayerControlledLocation(FVector::ZeroVector);
			}
			else if (dist > 150.0F)
			{
				MoveToLocation(GetPlayerControlledLocation());
			}
			else
			{
				SetPlayerControlledLocation(FVector::ZeroVector);
			}
		}
		else
		{
			Super::Tick(DeltaTime);

			// If we have no valid target, check if we should move to the player
			if (!IsTargetValid(GetTarget(), EAffectedTeam::Enemy) 
				&& !IsTargetValid(GetTarget(), EAffectedTeam::Ally))
			{
				AMech_RPGCharacter* player = GetAIOwner()->GetGroup()->GetPlayer();
				if (ShouldMoveToPlayer(player))
				{
					MoveToActor(player);
				}
				else
				{
					StopMovement();
				}
			}
		}
	}
}

bool AAllyAIController::ShouldMoveToPlayer(AMech_RPGCharacter* player)
{
	if (player != nullptr)
	{
		int32 indexOfByKey = GetAIOwner()->GetGroup()->GetMembers().IndexOfByKey(GetAIOwner());

		// This will try and spread the bots out a bit based on how many there are
		float distance = player->IsInCombat() ? GetAIOwner()->GetCurrentWeapon()->GetRange() : 300.0F + (indexOfByKey * 60);

		return player->GetDistanceTo(GetAIOwner()) > distance;
	}
	return false;
}

void AAllyAIController::BeginPlay()
{
	Super::BeginPlay();
	SetPlayerControlledLocation(FVector::ZeroVector);
}

FVector AAllyAIController::GetPlayerControlledLocation()
{
	return playerControlledLocation;
}

void AAllyAIController::SetPlayerControlledLocation(FVector newVal)
{
	playerControlledLocation = newVal;
}

