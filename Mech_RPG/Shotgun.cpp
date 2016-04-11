// Fill out your copyright notice in the Description page of Project Settings.

#include "Mech_RPG.h"
#include "Shotgun.h"

AShotgun::AShotgun() : Super() {
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Shotgun(TEXT("/Game/TopDown/Meshes/Shotgun"));
	if (Shotgun.Succeeded()) {
		mesh = Shotgun.Object;
	}
}

AShotgun* AShotgun::CreateShotgun(UWorld* world, AMech_RPGCharacter* inOwner) {
	if (world != nullptr) {
		FMagazineWeaponParams magSettings;
		magSettings.healthChange = 150;
		magSettings.range = 400;
		magSettings.fireRate = 0.8;
		magSettings.heals = false;
		magSettings.magazineSize = 5;
		magSettings.reloadAmount = 1;

		AShotgun* weapon = world->SpawnActor<AShotgun>(AShotgun::StaticClass());
		weapon->SetSettings(magSettings);
		weapon->SetOwner(inOwner);
		weapon->SetName("Shotgun");
		weapon->magazineSize = magSettings.magazineSize;
		weapon->reloadAmount = magSettings.reloadAmount;
		weapon->ammo = magSettings.magazineSize;
		return weapon;
	}
	return NULL;
}

AItem* AShotgun::Copy()
{
	AShotgun* weapon = GetWorld()->SpawnActor<AShotgun>(AShotgun::StaticClass());
	weapon->SetSettings(settings);
	weapon->SetOwner(GetOwner());
	weapon->magazineSize = magazineSize;
	weapon->reloadAmount = reloadAmount;
	weapon->ammo = magazineSize;
	weapon->SetName(GetName());
	return weapon;
}
