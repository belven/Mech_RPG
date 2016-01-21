// Fill out your copyright notice in the Description page of Project Settings.

#include "Mech_RPG.h"
#include "AOEHealthChange.h"
#include "Mech_RPGCharacter.h"
#include "DrawDebugHelpers.h"

AAOEHealthChange::AAOEHealthChange() : Super() {
	partclSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("AoEParticleSystem"));

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleSystemClass(TEXT("/Game/TopDown/Particle_Effects/AoE"));
	if (ParticleSystemClass.Succeeded()) {
		partclSystem->Template = ParticleSystemClass.Object;
		partclSystem->bAutoActivate = false;
		SetRootComponent(partclSystem);
	}

	SetActorHiddenInGame(false);
}

AAOEHealthChange* AAOEHealthChange::CreateAOEHealthChange(FTempAOESettings inSettings) {
	FVector locationToUse;

	if (inSettings.usesTarget && inSettings.target != NULL) {
		locationToUse = inSettings.target->GetActorLocation();
		locationToUse.Z -= inSettings.target->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	}
	else {
		locationToUse = inSettings.location;
	}

	AAOEHealthChange* tempAOE = inSettings.world->SpawnActor<AAOEHealthChange>(StaticClass(), locationToUse, FRotator(90, 0, 0));
	tempAOE->settings = inSettings;
	tempAOE->Activate();
	return tempAOE;
}

void  AAOEHealthChange::Activate() {
	if (timesRan < settings.duration) {
		timesRan++;

		FHealthChange damage;
		FVector locationToUse = settings.usesTarget && settings.target != NULL ? settings.target->GetActorLocation() : settings.location;
		//DrawDebugSphere(settings.world, locationToUse, settings.radius, 10, FColor::Blue, false, settings.rate, 0);

		if (!partclSystem->IsActive()) {
			partclSystem->SetVectorParameter(FName(TEXT("Size")), FVector(settings.radius * 2));
			partclSystem->Activate(true);
		}

		damage.damager = settings.owner;

		for (AMech_RPGCharacter* character : UMiscLibrary::GetCharactersInRange(settings.radius, locationToUse)) {
			bool canAffect = settings.affectedTeam == AOEEnums::Ally ? character->CompareGroup(settings.owner) : !character->CompareGroup(settings.owner);

			if (canAffect) {
				float tempDamage = settings.healthChange > 2 ? settings.healthChange : character->GetMaxHealth() * settings.healthChange;
				damage.healthChange = tempDamage;
				damage.target = character;
				damage.damageType = settings.damageType;
				character->ChangeHealth(damage);
			}
		}

		settings.world->GetTimerManager().SetTimer(TimerHandle_AOERate, this, &AAOEHealthChange::Activate, settings.rate);
	}
	else {
		partclSystem->Deactivate();
	}
}
