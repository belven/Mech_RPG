// Fill out your copyright notice in the Description page of Project Settings.

#include "Mech_RPG.h"
#include "Sniper.h"

USniper::USniper() : Super() {
	static ConstructorHelpers::FObjectFinder<UStaticMesh> sniper(TEXT("/Game/TopDown/Meshes/Weapons/Sniper"));
	if (sniper.Succeeded()) {
		mesh = sniper.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundCue> soundClass(TEXT("/Game/TopDown/Sounds/Sniper_Cue.Sniper_Cue"));

	if (soundClass.Succeeded()) {
		audioComp->SetSound(soundClass.Object);
	}
}

USniper* USniper::CreateSniper(AMech_RPGCharacter* inOwner) {
		FMagazineWeaponParams magSettings;
		magSettings.healthChange = 300;
		magSettings.range = 1500;
		magSettings.fireRate = 1.5;
		magSettings.heals = false;
		magSettings.magazineSize = 5;
		magSettings.reloadAmount = 1;
		magSettings.critChance = 45;

		USniper* weapon = NewObject<USniper>(StaticClass());
		weapon->SetSettings(magSettings);
		weapon->SetItemOwner(inOwner);
		weapon->SetName("Sniper");
		weapon->magazineSize = magSettings.magazineSize;
		weapon->reloadAmount = magSettings.reloadAmount;
		weapon->ammo = magSettings.magazineSize;
		return weapon;
}

UItem* USniper::Copy()
{
	USniper* weapon = NewObject<USniper>(StaticClass());
	weapon->SetSettings(settings);
	weapon->CloneItemSettings(this);
	weapon->magazineSize = magazineSize;
	weapon->reloadAmount = reloadAmount;
	weapon->ammo = magazineSize;
	return weapon;
}
