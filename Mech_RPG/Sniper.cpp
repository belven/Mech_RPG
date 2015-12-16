// Fill out your copyright notice in the Description page of Project Settings.

#include "Mech_RPG.h"
#include "Sniper.h"

ASniper* ASniper::CreateSniper(AActor* inOwner) {
	if (inOwner && inOwner->GetWorld()) {
		FMagazineWeaponParams magSettings;
		magSettings.damage = 250;
		magSettings.range = 1500;
		magSettings.fireRate = 2;
		magSettings.heals = false;
		magSettings.magazineSize = 5;
		magSettings.reloadAmount = 1;
		magSettings.critChance = 45;

		ASniper* weapon = inOwner->GetWorld()->SpawnActor<ASniper>(ASniper::StaticClass());
		weapon->settings = magSettings;
		weapon->canFire = true;
		weapon->AttachRootComponentToActor(inOwner);
		weapon->lastTime = 0;
		return weapon;
	}
	return NULL;
}