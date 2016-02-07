// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "Bio_Rifle.h"

ABio_Rifle::ABio_Rifle() : Super() {
	static ConstructorHelpers::FObjectFinder<UStaticMesh> sniper(TEXT("/Game/TopDown/Meshes/Bio_Rifle"));
	if (sniper.Succeeded()) {
		mesh = sniper.Object;
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleSystemClass(TEXT("/Game/TopDown/Particle_Effects/Bio_beam"));
	if (ParticleSystemClass.Succeeded()) {
		partclSystem->Template = ParticleSystemClass.Object;
		partclSystem->bAutoActivate = false;
		partclSystem->SetActorParameter(FName(TEXT("BeamSource")), this);
	}
}

ABio_Rifle* ABio_Rifle::CreateBioRifle(AMech_RPGCharacter* inOwner) {
	if (inOwner && inOwner->GetWorld()) {
		FOverheatWeaponParams overheatSettings;
		overheatSettings.healthChange = 40;
		overheatSettings.range = 600;
		overheatSettings.fireRate = 0.3;
		overheatSettings.heals = true;
		overheatSettings.heatLosePerTick = 0.05;
		overheatSettings.heatGenerated = 0.05;

		ABio_Rifle* weapon = inOwner->GetWorld()->SpawnActor<ABio_Rifle>(ABio_Rifle::StaticClass());
		weapon->SetSettings(overheatSettings);
		weapon->heatLosePerTick = overheatSettings.heatLosePerTick;
		weapon->heatGenerated = overheatSettings.heatGenerated;
		weapon->SetOwner(inOwner);
		return weapon;
	}
	return NULL;
}


