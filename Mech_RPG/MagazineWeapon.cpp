// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "MagazineWeapon.h"

void AMagazineWeapon::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	lastTime += DeltaTime;
	
	if (ammo <= 0) {
		reloading = true;
	}

	if (reloading && lastTime >= 1) {
		lastTime = 0;

		ammo += reloadAmount;

		if (ammo >= magazineSize) {
			reloading = false;
			ammo = magazineSize;
		}
	}
}

bool AMagazineWeapon::CanFire() {
	return !reloading && Super::CanFire();
}


void AMagazineWeapon::Fire(class AMech_RPGCharacter* target, AMech_RPGCharacter* owner) {
	Super::Fire(target, owner);
	ammo--;
}


AMagazineWeapon* AMagazineWeapon::CreateMagazineWeapon(AActor* inOwner, FMagazineWeaponParams inSettings) {
	if (inOwner && inOwner->GetWorld()) {
		AMagazineWeapon* weapon = inOwner->GetWorld()->SpawnActor<AMagazineWeapon>(AMagazineWeapon::StaticClass());
		weapon->settings = inSettings;
		weapon->canFire = true;
		weapon->AttachRootComponentToActor(inOwner);
		weapon->lastTime = 0;
		weapon->magazineSize = inSettings.magazineSize;
		weapon->reloadAmount = inSettings.reloadAmount;
		return weapon;
	}
	return NULL;
}