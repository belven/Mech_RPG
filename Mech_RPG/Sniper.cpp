// Fill out your copyright notice in the Description page of Project Settings.

#include "Mech_RPG.h"
#include "Sniper.h"

ASniper::ASniper() : Super() {
	static ConstructorHelpers::FObjectFinder<UStaticMesh> sniper(TEXT("/Game/TopDown/Meshes/Sniper"));
	if (sniper.Succeeded()) {
		mesh = sniper.Object;
	}	 	
}

ASniper* ASniper::CreateSniper(AMech_RPGCharacter* inOwner) {
	if (inOwner && inOwner->GetWorld()) {
		FMagazineWeaponParams magSettings;
		magSettings.healthChange = 300;
		magSettings.range = 1500;
		magSettings.fireRate = 1.5;
		magSettings.heals = false;
		magSettings.magazineSize = 5;
		magSettings.reloadAmount = 1;
		magSettings.critChance = 45;

		ASniper* weapon = inOwner->GetWorld()->SpawnActor<ASniper>(ASniper::StaticClass());
		weapon->settings = magSettings;
		weapon->canFire = true;
		//weapon->AttachRootComponentToActor(inOwner);
		weapon->lastTime = 0;
		weapon->SetOwner(inOwner);
		weapon->GetOwner()->OnStopFiring.AddDynamic(weapon, &AWeapon::StopFire);
		return weapon;
	}
	return NULL;
}