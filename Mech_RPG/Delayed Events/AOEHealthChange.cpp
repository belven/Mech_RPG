// Fill out your copyright notice in the Description page of Project Settings.

#include "Mech_RPG.h"
#include "AOEHealthChange.h"
#include "Characters/Mech_RPGCharacter.h"
#include "DrawDebugHelpers.h"

AAOEHealthChange::AAOEHealthChange()
	: currentLifetime(0),
	Super() {
	partclSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("AoEParticleSystem"));

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleSystemClass(TEXT("/Game/TopDown/Particle_Effects/AoE"));
	if (ParticleSystemClass.Succeeded()) {
		partclSystem->Template = ParticleSystemClass.Object;
		partclSystem->bAutoActivate = false;
		//partclSystem->SetWorldRotation(FRotator(0, 0, 0));
		//partclSystem->SetRelativeRotation(FRotator(0, 0, 0));
		SetRootComponent(partclSystem);
	}

	SetActorHiddenInGame(false);
}

AAOEHealthChange* AAOEHealthChange::CreateAOEHealthChange(FTempAOESettings inSettings) {
	AAOEHealthChange* tempAOE = inSettings.world->SpawnActor<AAOEHealthChange>(StaticClass(), GetLocationToUse(inSettings), FRotator(0, 0, 0));
	tempAOE->settings = inSettings;
	tempAOE->Activate();
	return tempAOE;
}

FVector AAOEHealthChange::GetLocationToUse(FTempAOESettings inSettings)
{
	FVector locationToUse;

	if (inSettings.usesTarget && inSettings.target != nullptr) {
		locationToUse = inSettings.target->GetActorLocation();
		locationToUse.Z -= inSettings.target->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	}
	else {
		locationToUse = inSettings.location;
	}
	return locationToUse;
}

bool AAOEHealthChange::CanAffect(AMech_RPGCharacter* character)
{
	return UMiscLibrary::IsTargetValid(settings.owner, character, settings.affectedTeam);
}

float AAOEHealthChange::GetHealthChange(AMech_RPGCharacter* character) {
	if (settings.healthChange < UMiscLibrary::MAX_HEALTH_CHANGE
		&& UMiscLibrary::IsCharacterAlive(character)) {
		return character->GetMaxHealth() * settings.healthChange;
	}
	return settings.healthChange;
}

void AAOEHealthChange::CreateDebugSphere(FVector locationToUse) {
	DrawDebugSphere(settings.world, locationToUse, settings.radius, 10, UMiscLibrary::GetRelativeColour(settings.owner), false, settings.rate, 0);
}


void  AAOEHealthChange::Activate() {
	if (currentLifetime <= settings.duration) {
		currentLifetime += settings.rate;

		FVector locationToUse = GetLocationToUse(settings);
		CreateDebugSphere(locationToUse);

		/*	if (!partclSystem->IsActive()) {
				partclSystem->SetVectorParameter(FName(TEXT("Size")), FVector(settings.radius * 2));
				partclSystem->Activate(true);
			}*/

		for (AMech_RPGCharacter* character : UMiscLibrary::GetCharactersInRange(settings.radius, locationToUse)) {
			if (CanAffect(character)) {
				PerformHealthChange(character);
			}
		}

		settings.world->GetTimerManager().SetTimer(TimerHandle_AOERate, this, &AAOEHealthChange::Activate, settings.rate);
	}
	else {
		partclSystem->Deactivate();
		Destroy();
	}
}

void AAOEHealthChange::PerformHealthChange(AMech_RPGCharacter* character)
{
	float tempDamage = GetHealthChange(character);
	FHealthChange healthChange;
	healthChange.manipulator = settings.owner;
	healthChange.changeAmount = tempDamage;
	healthChange.target = character;
	healthChange.damageType = settings.damageType;
	healthChange.heals = settings.heals;
	character->ChangeHealth(healthChange);
}
