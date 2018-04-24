// Fill out your copyright notice in the Description page of Project Settings.

#include "Mech_RPG.h"
#include "SMG.h"

USMG::USMG() : Super()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> smg(TEXT("/Game/TopDown/Meshes/Weapons/SMG"));
	if (smg.Succeeded())
	{
		mesh = smg.Object;
	}
}


USMG* USMG::CreateSMG(AMech_RPGCharacter* inOwner)
{
	FMagazineWeaponParams magSettings;
	magSettings.healthChange = 40;
	magSettings.range = 1000;
	magSettings.fireRate = 0.1F;
	magSettings.heals = false;
	magSettings.magazineSize = 60;
	magSettings.reloadAmount = 15;

	USMG* weapon = NewObject<USMG>(StaticClass());
	weapon->SetSettings(magSettings);
	weapon->SetItemOwner(inOwner);
	weapon->SetName("SMG");
	weapon->magazineSize = magSettings.magazineSize;
	weapon->reloadAmount = magSettings.reloadAmount;
	weapon->ammo = magSettings.magazineSize;
	return weapon;
}

UItem* USMG::Copy()
{
	USMG* weapon = NewObject<USMG>(StaticClass());
	weapon->SetSettings(settings);
	weapon->magazineSize = magazineSize;
	weapon->reloadAmount = reloadAmount;
	weapon->ammo = magazineSize;
	weapon->CloneItemSettings(this);
	return weapon;
}