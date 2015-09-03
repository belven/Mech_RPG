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
			if (FVector::Dist(GetPlayerControlledLocation(), GetOwner()->GetActorLocation()) > 200) {
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
				AMech_RPGCharacter* player = GetOwner()->GetGroup()->GetPlayer();
				if (player != NULL && player->GetDistanceTo(GetOwner()) > 400) {
					GetWorld()->GetNavigationSystem()->SimpleMoveToActor(this, player);
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
	if (!GetOwner()->GetCurrentWeapon()->Heals()) {
		for (FConstPawnIterator iter = GetWorld()->GetPawnIterator(); iter; iter++) {
			APawn* pawn = iter->Get();
			if (pawn != NULL && pawn != GetOwner() && pawn->GetDistanceTo(GetOwner()) <= GetOwner()->GetCurrentWeapon()->GetRange()) {
				AMech_RPGCharacter* character = Cast<AMech_RPGCharacter>(pawn);

				if (!character->IsDead() && !character->CompareGroup(GetOwner())) {
					SetTarget(character);
					break;
				}
			}
		}
	}
	else {
		for (AMech_RPGCharacter* character : GetOwner()->GetGroup()->GetMembers()) {
			if (!character->IsDead() && character->GetHealth() < character->GetMaxHealth()) {
				SetTarget(character);
				break;
			}
		}
	}
}