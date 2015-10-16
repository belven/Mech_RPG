// Copyright of Explosive Industries

#include "Mech_RPG.h"

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
void AOverHeatWeapon::Fire(class AMech_RPGCharacter* target, AMech_RPGCharacter* owner) {
	float damageDealt = settings.damage * owner->GetDamageModifier();
	float actualDamageDealt = 0;

	if (Heals()) {
		// Calculate actual amount healed based on missing health and max health
		float missingHealth = target->GetMaxHealth() - target->GetHealth();

		if (settings.damage > missingHealth) {
			actualDamageDealt = missingHealth;
		}
		else {
			actualDamageDealt = settings.damage;
		}
	}
	else {
		//Calculate actual damage dealt based on health less and max health
		float remainingHealth = target->GetHealth();

		if (settings.damage > remainingHealth) {
			actualDamageDealt = remainingHealth;
		}
		else {
			actualDamageDealt = settings.damage;
		}
	}

	heatLevel += (actualDamageDealt / target->GetMaxHealth()) * (1 + heatLevel);
	Super::Fire(target, owner);
}


AOverHeatWeapon* AOverHeatWeapon::CreateOverHeatWeapon(AActor* inOwner, FWeaponParams inSettings) {
	if (inOwner && inOwner->GetWorld()) {
		AOverHeatWeapon* weapon = inOwner->GetWorld()->SpawnActor<AOverHeatWeapon>(AOverHeatWeapon::StaticClass());
		weapon->settings = inSettings;
		weapon->canFire = true;
		weapon->AttachRootComponentToActor(inOwner);
		weapon->lastTime = 0;
		weapon->overHeated = false;
		weapon->heatLevel = 0;
		weapon->heatLosePerTick = 0.05;
		return weapon;
	}
	return NULL;
}