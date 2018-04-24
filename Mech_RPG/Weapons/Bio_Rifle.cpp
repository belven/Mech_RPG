// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "Bio_Rifle.h"

UBio_Rifle::UBio_Rifle() : Super()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> sniper(TEXT("/Game/TopDown/Meshes/Weapons/Bio_Rifle"));
	if (sniper.Succeeded())
	{
		mesh = sniper.Object;
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleSystemClass(TEXT("/Game/TopDown/Particle_Effects/Bio_beam"));
	if (ParticleSystemClass.Succeeded())
	{
		partclSystem->Template = ParticleSystemClass.Object;
		partclSystem->bAutoActivate = false;
		//partclSystem->SetActorParameter(FName(TEXT("BeamSource")), meshComponent);
	}


	static ConstructorHelpers::FObjectFinder<USoundCue> soundClass(TEXT("/Game/TopDown/Sounds/Bio_Rifle_Cue.Bio_Rifle_Cue"));

	if (soundClass.Succeeded())
	{
		audioComp->SetSound(soundClass.Object);
	}
}

UBio_Rifle* UBio_Rifle::CreateBioRifle(AMech_RPGCharacter* inOwner)
{
		FOverheatWeaponParams overheatSettings;
		overheatSettings.healthChange = 40;
		overheatSettings.range = 1000;
		overheatSettings.fireRate = 0.3;
		overheatSettings.heals = true;
		overheatSettings.heatLosePerTick = 0.05;
		overheatSettings.heatGenerated = 0.05;

		UBio_Rifle* weapon = NewObject<UBio_Rifle>(StaticClass());
		weapon->SetSettings(overheatSettings);
		weapon->heatLosePerTick = overheatSettings.heatLosePerTick;
		weapon->heatGenerated = overheatSettings.heatGenerated;
		weapon->SetItemOwner(inOwner);
		weapon->SetName("Bio Rifle");
		return weapon;
}

UItem* UBio_Rifle::Copy()
{
	UBio_Rifle* weapon = NewObject<UBio_Rifle>(StaticClass());
	weapon->SetSettings(settings);
	weapon->heatLosePerTick = heatLosePerTick;
	weapon->heatGenerated = heatGenerated;
	weapon->CloneItemSettings(this);
	return weapon;
}

