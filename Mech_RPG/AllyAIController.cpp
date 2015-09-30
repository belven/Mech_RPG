#pragma once
#include "Mech_RPG.h"
#include "AllyAIController.h"
#include "Mech_RPGCharacter.h"
#include "Group.h"
#include "AI/Navigation/NavigationSystem.h"

void AAllyAIController::Tick(float DeltaTime) {
	if (GetOwner() && GetOwner()->GetDemandedController() == NULL) {
		if (GetOwner()->IsDead()) {
			UnPossess();
			//GetOwner()->Destroy(true);
		}
		else if (GetPlayerControlledLocation() != FVector::ZeroVector) {
			float dist = FVector::Dist(GetPlayerControlledLocation(), GetOwner()->GetActorLocation());
			if (dist > 1800.0F) {
				SetPlayerControlledLocation(FVector::ZeroVector);
			}
			else if (dist > 150) {
				MoveToLocation(GetPlayerControlledLocation());
			}
			else {
				SetPlayerControlledLocation(FVector::ZeroVector);

				FindTargetInWeaponRage();

				if (IsTargetValid(GetTarget())) {
					AttackTarget(DeltaTime);
				}
			}
		}
		else {
			if (!IsTargetValid(GetTarget())) {
				FindTarget();
			}

			if (IsTargetValid(GetTarget())) {
				AttackTarget(DeltaTime);
			}
			else {
				AMech_RPGCharacter* player = GetOwner()->GetGroup()->GetPlayer();
				if (player != NULL && player->GetDistanceTo(GetOwner()) > 300.0F) {
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
	SetPlayerControlledLocation(FVector::ZeroVector);
}

FVector AAllyAIController::GetPlayerControlledLocation() {
	return playerControlledLocation;
}

void AAllyAIController::SetPlayerControlledLocation(FVector newVal) {
	playerControlledLocation = newVal;
}

void AAllyAIController::FindTargetInWeaponRage() {
	float weaponRange = GetOwner()->GetCurrentWeapon()->GetRange();

	if (!GetOwner()->GetCurrentWeapon()->Heals()) {
		for (AMech_RPGCharacter* character : GetCharactersInRange(weaponRange)) {
			if (!character->CompareGroup(GetOwner())) {
				SetTarget(character);
				break;
			}
		}
	}
	else {
		for (AMech_RPGCharacter* character : GetOwner()->GetGroup()->GetMembers()) {
			if (!character->IsDead()
				&& character->GetHealth() < character->GetMaxHealth()
				&& character->GetDistanceTo(GetOwner()) <= weaponRange) {
				SetTarget(character);
				break;
			}
		}
	}
}