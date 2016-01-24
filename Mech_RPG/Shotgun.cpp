// Fill out your copyright notice in the Description page of Project Settings.

#include "Mech_RPG.h"
#include "Shotgun.h"

AShotgun::AShotgun() : Super() {
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Shotgun(TEXT("/Game/TopDown/Meshes/Shotgun"));
	if (Shotgun.Succeeded()) {
		mesh = Shotgun.Object;
	}
}

AShotgun* AShotgun::CreateShotgun(AMech_RPGCharacter* inOwner) {
	if (inOwner && inOwner->GetWorld()) {
		FMagazineWeaponParams magSettings;
		magSettings.healthChange = 150;
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
		weapon->SetOwner(inOwner);
		weapon->GetOwner()->OnStopFiring.AddDynamic(weapon, &AWeapon::StopFire);
		return weapon;
	}
	return NULL;
}