// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "AllyAIController.h"
#include "Drone.h"

ADrone::ADrone() : Super()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> newMesh(TEXT("/Game/TopDown/Meshes/Drone.Drone"));

	if (newMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(newMesh.Object);
	}

	AIControllerClass = AAllyAIController::StaticClass();
}

void ADrone::CreatePresetRole(ERole inRole, int32 grade, int32 quaility)
{
	Super::CreatePresetRole(ERole::Drone, grade, quaility);
}

float ADrone::GetMaxHealth()
{
	if (droneOwner != nullptr) {
		return maxHealth * (1 + droneOwner->GetStatBonus(EStatEnum::DroneHealth));
	}
	else return maxHealth;
}

float ADrone::GetHealthChangeModifier()
{
	if (droneOwner != nullptr) {
		return healthChangeModifier * (1 + droneOwner->GetStatBonus(EStatEnum::DroneDamage));
	}
	else return healthChangeModifier;
}

float ADrone::GetDefenceModifier()
{
	if (droneOwner != nullptr) {
		return MIN(defenceModifier + droneOwner->GetStatBonus(EStatEnum::DroneDefence), 0.99F);
	}
	else return defenceModifier;
}

