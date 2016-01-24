// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "Mech_RPGCharacter.h"

void AMagazineWeapon::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	if (ammo <= 0) {
		reloading = true;
		StopFire();
	}

	if (reloading) {
		lastTime += DeltaTime;

		if (lastTime >= 1) {
			lastTime = 0;

			ammo += reloadAmount;

			if (ammo >= magazineSize) {
				reloading = false;
				ammo = magazineSize;
			}
		}
	}
}

void AMagazineWeapon::SetOwner(AMech_RPGCharacter* inOwner) {
	Super::SetOwner(inOwner);
	inOwner->OnOutOfCombat.AddUniqueDynamic(this, &AMagazineWeapon::FullReload);
}

void AMagazineWeapon::FullReload() {
	reloading = false;
	ammo = magazineSize;
}

bool AMagazineWeapon::CanFire() {
	return !reloading && Super::CanFire();
}

float AMagazineWeapon::GetMagazineSize() {
	return magazineSize;
}

float AMagazineWeapon::GetProgressBarPercent() {
	return ammo / magazineSize;
}

float AMagazineWeapon::GetAmmo() {
	return ammo;
}

void AMagazineWeapon::Fire(AMech_RPGCharacter* target) {
	Super::Fire(target);
	ammo--;
}


AMagazineWeapon* AMagazineWeapon::CreateMagazineWeapon(AMech_RPGCharacter* inOwner, FMagazineWeaponParams inSettings) {
	if (inOwner && inOwner->GetWorld()) {
		AMagazineWeapon* weapon = inOwner->GetWorld()->SpawnActor<AMagazineWeapon>(AMagazineWeapon::StaticClass());
		weapon->settings = inSettings;
		weapon->canFire = true;
		weapon->AttachRootComponentToActor(inOwner);
		weapon->lastTime = 0;
		weapon->magazineSize = inSettings.magazineSize;
		weapon->reloadAmount = inSettings.reloadAmount;
		weapon->ammo = inSettings.magazineSize;
		weapon->reloading = false;
		weapon->SetOwner(inOwner);
		weapon->GetOwner()->OnStopFiring.AddDynamic(weapon, &AWeapon::StopFire);
		return weapon;
	}
	return NULL;
}