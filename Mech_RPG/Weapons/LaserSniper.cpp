// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "LaserSniper.h"

ALaserSniper::ALaserSniper()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> sniper(TEXT("/Game/TopDown/Meshes/Weapons/Sniper"));
	if (sniper.Succeeded()) {
		mesh = sniper.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundCue> soundClass(TEXT("/Game/TopDown/Sounds/Sniper_Cue.Sniper_Cue"));

	if (soundClass.Succeeded()) {
		audioComp->SetSound(soundClass.Object);
	}
}

ALaserSniper * ALaserSniper::CreateLaserSniper(UWorld * world, AMech_RPGCharacter * inOwner)
{
	if (world != nullptr) {
		FOverheatWeaponParams overheatSettings;
		overheatSettings.healthChange = 300;
		overheatSettings.range = 2000;
		overheatSettings.fireRate = 1.5;
		overheatSettings.heals = false;
		overheatSettings.heatLosePerTick = 0.05;
		overheatSettings.heatGenerated = 0.15;
		overheatSettings.critChance = 45;
		overheatSettings.damageType = EDamageType::Energy;

		ALaserSniper* weapon = world->SpawnActor<ALaserSniper>(ALaserSniper::StaticClass());
		weapon->SetSettings(overheatSettings);
		weapon->heatLosePerTick = overheatSettings.heatLosePerTick;
		weapon->heatGenerated = overheatSettings.heatGenerated;
		weapon->SetItemOwner(inOwner);
		weapon->SetName("Laser Sniper");
		return weapon;
	}
	return NULL;
}

AItem* ALaserSniper::Copy()
{
	ALaserSniper* weapon = GetWorld()->SpawnActor<ALaserSniper>(ALaserSniper::StaticClass());
	weapon->SetSettings(settings);
	weapon->SetItemOwner(GetItemOwner());
	weapon->heatLosePerTick = heatLosePerTick;
	weapon->heatGenerated = heatGenerated;
	weapon->SetName(GetName());
	return weapon;

}
