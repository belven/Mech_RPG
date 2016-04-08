// Fill out your copyright notice in the Description page of Project Settings.

#include "Mech_RPG.h"
#include "SMG.h"

ASMG::ASMG() : Super() {
	static ConstructorHelpers::FObjectFinder<UStaticMesh> smg(TEXT("/Game/TopDown/Meshes/SMG"));
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
		return weapon;
	}
	return NULL;
}

