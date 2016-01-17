// Fill out your copyright notice in the Description page of Project Settings.

#include "Mech_RPG.h"
#include "AOEHealthChange.h"
#include "Mech_RPGCharacter.h"
#include "DrawDebugHelpers.h"

UAOEHealthChange* UAOEHealthChange::CreateAOEHealthChange(FTempAOESettings inSettings) {
	UAOEHealthChange* tempAOE = NewObject<UAOEHealthChange>(UAOEHealthChange::StaticClass());
	tempAOE->settings = inSettings;
	tempAOE->Activate();
	return tempAOE;
}

void  UAOEHealthChange::Activate() {
	if (timesRan < settings.duration) {
		timesRan++;

		FHealthChange damage;
		FVector locationToUse = settings.usesTarget && settings.target != NULL ? settings.target->GetActorLocation() : settings.location;
		DrawDebugSphere(settings.world, locationToUse, settings.radius, 10, FColor::Blue, false, settings.rate, 0);

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

		settings.world->GetTimerManager().SetTimer(TimerHandle_AOERate, this, &UAOEHealthChange::Activate, settings.rate);
	}
}
