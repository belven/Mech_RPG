// Fill out your copyright notice in the Description page of Project Settings.

#include "Mech_RPG.h"
#include "Shotgun.h"
#include "Characters/Mech_RPGCharacter.h"

UShotgun::UShotgun() : Super()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Shotgun(TEXT("/Game/TopDown/Meshes/Weapons/Shotgun"));
	if (Shotgun.Succeeded())
	{
		mesh = Shotgun.Object;
	}
}

UShotgun* UShotgun::CreateShotgun(AMech_RPGCharacter* inOwner)
{
	FMagazineWeaponParams magSettings;
	magSettings.healthChange = 150;
	magSettings.range = 400;
	magSettings.fireRate = 0.8;
	magSettings.heals = false;
	magSettings.magazineSize = 5;
	magSettings.reloadAmount = 1;

	UShotgun* weapon = NewObject<UShotgun>(StaticClass());
	weapon->SetSettings(magSettings);
	weapon->SetItemOwner(inOwner);
	weapon->SetName("Shotgun");
	weapon->magazineSize = magSettings.magazineSize;
	weapon->reloadAmount = magSettings.reloadAmount;
	weapon->ammo = magSettings.magazineSize;
	return weapon;
}

UItem* UShotgun::Copy()
{
	UShotgun* weapon = NewObject<UShotgun>(StaticClass());
	weapon->SetSettings(settings);
	weapon->magazineSize = magazineSize;
	weapon->reloadAmount = reloadAmount;
	weapon->ammo = magazineSize;
	weapon->CloneItemSettings(this);
	return weapon;
}
