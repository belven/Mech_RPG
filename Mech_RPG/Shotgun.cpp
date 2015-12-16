// Fill out your copyright notice in the Description page of Project Settings.

#include "Mech_RPG.h"
#include "Shotgun.h"




AShotgun* AShotgun::CreateShotgun(AActor* inOwner) {
	if (inOwner && inOwner->GetWorld()) {
		FMagazineWeaponParams magSettings;
		magSettings.damage = 150;
		magSettings.range = 400;
		magSettings.fireRate = 0.8;
		magSettings.heals = false;
		magSettings.magazineSize = 5;
		magSettings.reloadAmount = 1;

		AShotgun* weapon = inOwner->GetWorld()->SpawnActor<AShotgun>(AShotgun::StaticClass());
		weapon->settings = magSettings;
		weapon->canFire = true;
		weapon->AttachRootComponentToActor(inOwner);
		weapon->lastTime = 0;
		return weapon;
	}
	return NULL;
}