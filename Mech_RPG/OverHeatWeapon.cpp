// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "Mech_RPGCharacter.h"
#include "OverHeatWeapon.h"

void AOverHeatWeapon::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (heatLevel >= 1) {
		overHeated = true;
		heatLevel = 1;
	}

	if (heatLevel > 0) {
		if (!overHeated) {
			// It takes 20 seconds to reduce 100 heat
			heatLevel -= heatLosePerTick * DeltaTime;
		}
		else {
			StopFire();
			// If takes 6.45 ish seconds to loose 100 heat when overHeated is TRUE
			heatLevel -= 0.12 * DeltaTime;

			if (heatLevel < 0) {
				heatLevel = 0;
				overHeated = false;
			}
		}
	}

	if (heatLevel < 0) {
		heatLevel = 0;
	}
}

float AOverHeatWeapon::GetProgressBarPercent() {
	return heatLevel;
}

void AOverHeatWeapon::SetOwner(AMech_RPGCharacter* inOwner) {
	Super::SetOwner(inOwner);
	if (inOwner != nullptr) {
		inOwner->OnOutOfCombat.AddUniqueDynamic(this, &AOverHeatWeapon::Cooldown);
	}
}

void AOverHeatWeapon::Cooldown() {
	heatLevel = 0;
	overHeated = false;
}

bool AOverHeatWeapon::CanFire() {
	return !overHeated && Super::CanFire();
}

float AOverHeatWeapon::GetHeatLevel() {
	return heatLevel;
}

/* Pre calculate the amount of damage
and add heat based on the amount of damage/healing dealt as a percent of the targets max health

This will cause healers to have a burnout allowing thier allies to be killed with only a few DPS
*/
void AOverHeatWeapon::Fire(AMech_RPGCharacter* target) {
	if ((Heals() && UMiscLibrary::GetMissingHealth(target) > 0) || (!Heals() && target->GetHealth() > 0)) {
		heatLevel += heatGenerated * (1 + (heatLevel * 0.5));
	}

	Super::Fire(target);
}

AOverHeatWeapon* AOverHeatWeapon::CreateOverHeatWeapon(UWorld* world, AMech_RPGCharacter* inOwner, FOverheatWeaponParams inSettings) {
	if (world != nullptr) {
		AOverHeatWeapon* weapon = world->SpawnActor<AOverHeatWeapon>(AOverHeatWeapon::StaticClass());
		weapon->SetSettings(inSettings);
		weapon->heatLosePerTick = inSettings.heatLosePerTick;
		weapon->heatGenerated = inSettings.heatGenerated;
		weapon->SetOwner(inOwner);
		return weapon;
	}
	return NULL;
}