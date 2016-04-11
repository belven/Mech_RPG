// Fill out your copyright notice in the Description page of Project Settings.

#include "Mech_RPG.h"
#include "Sniper.h"

ASniper::ASniper() : Super() {
	static ConstructorHelpers::FObjectFinder<UStaticMesh> sniper(TEXT("/Game/TopDown/Meshes/Sniper"));
	if (sniper.Succeeded()) {
		mesh = sniper.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundCue> soundClass(TEXT("/Game/TopDown/Sounds/Sniper_Cue.Sniper_Cue"));

	if (soundClass.Succeeded()) {
		audioComp->SetSound(soundClass.Object);
	}
}

ASniper* ASniper::CreateSniper(UWorld* world, AMech_RPGCharacter* inOwner) {
	if (world != nullptr) {
		FMagazineWeaponParams magSettings;
		magSettings.healthChange = 300;
		magSettings.range = 1500;
		magSettings.fireRate = 1.5;
		magSettings.heals = false;
		magSettings.magazineSize = 5;
		magSettings.reloadAmount = 1;
		magSettings.critChance = 45;

		ASniper* weapon = world->SpawnActor<ASniper>(ASniper::StaticClass());
		weapon->SetSettings(magSettings);
		weapon->SetOwner(inOwner);
		weapon->SetName("Sniper");
		weapon->magazineSize = magSettings.magazineSize;
		weapon->reloadAmount = magSettings.reloadAmount;
		weapon->ammo = magSettings.magazineSize;
		return weapon;
	}
	return NULL;
}

AItem* ASniper::Copy()
{
	ASniper* weapon = GetWorld()->SpawnActor<ASniper>(ASniper::StaticClass());
	weapon->SetSettings(settings);
	weapon->SetOwner(GetOwner());
	weapon->SetName(GetName());
	weapon->magazineSize = magazineSize;
	weapon->reloadAmount = reloadAmount;
	weapon->ammo = magazineSize;
	return weapon;
}
