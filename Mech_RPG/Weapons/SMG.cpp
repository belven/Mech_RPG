// Fill out your copyright notice in the Description page of Project Settings.

#include "Mech_RPG.h"
#include "SMG.h"

ASMG::ASMG() : Super() {
	static ConstructorHelpers::FObjectFinder<UStaticMesh> smg(TEXT("/Game/TopDown/Meshes/Weapons/SMG"));
	if (smg.Succeeded()) {
		mesh = smg.Object;
	}
}


ASMG* ASMG::CreateSMG(UWorld* world, AMech_RPGCharacter* inOwner) {
	if (world != nullptr) {
		FMagazineWeaponParams magSettings;
		magSettings.healthChange = 42;
		magSettings.range = 1000;
		magSettings.fireRate = 0.3;
		magSettings.heals = false;
		magSettings.magazineSize = 20;
		magSettings.reloadAmount = 4;

		ASMG* weapon = world->SpawnActor<ASMG>(ASMG::StaticClass());
		weapon->SetSettings(magSettings);
		weapon->SetOwner(inOwner);
		weapon->SetName("SMG");
		weapon->magazineSize = magSettings.magazineSize;
		weapon->reloadAmount = magSettings.reloadAmount;
		weapon->ammo = magSettings.magazineSize;
		return weapon;
	}
	return NULL;
}

AItem* ASMG::Copy()
{
	ASMG* weapon = GetWorld()->SpawnActor<ASMG>(ASMG::StaticClass());
	weapon->SetSettings(settings);
	weapon->magazineSize = magazineSize;
	weapon->reloadAmount = reloadAmount;
	weapon->ammo = magazineSize;
	weapon->CloneItemSettings(this);
	return weapon;
}

