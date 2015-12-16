// Fill out your copyright notice in the Description page of Project Settings.

#include "Mech_RPG.h"
#include "SMG.h"


ASMG* ASMG::CreateSMG(AActor* inOwner) {
	if (inOwner && inOwner->GetWorld()) {
		FMagazineWeaponParams magSettings;
		magSettings.damage = 42;
		magSettings.range = 1000;
		magSettings.fireRate = 0.3;
		magSettings.heals = false;
		magSettings.magazineSize = 20;
		magSettings.reloadAmount = 4;

		ASMG* weapon = inOwner->GetWorld()->SpawnActor<ASMG>(ASMG::StaticClass());
		weapon->settings = magSettings;
		weapon->canFire = true;
		weapon->AttachRootComponentToActor(inOwner);
		weapon->lastTime = 0;
		return weapon;
	}
	return NULL;
}

