// Fill out your copyright notice in the Description page of Project Settings.

#include "Mech_RPG.h"
#include "AOEHealthChange.h"
#include "Mech_RPGCharacter.h"

UAOEHealthChange* UAOEHealthChange::CreateTestAOE(FTempAOESettings inSettings) {
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

		damage.damager = settings.owner;

		for (FConstPawnIterator iter = settings.world->GetPawnIterator(); iter; iter++) {
			APawn* pawn = iter->Get();

			if (iter->IsValid() 
				&& pawn != NULL 
				&& pawn->GetClass()->IsChildOf(AMech_RPGCharacter::StaticClass())) {

				AMech_RPGCharacter* character = Cast<AMech_RPGCharacter>(pawn);

				float tempDamage = settings.healthChange > 2 ? settings.healthChange : character->GetMaxHealth() * settings.healthChange;
				float dist = FVector::Dist(locationToUse, character->GetActorLocation());
				bool inRange = dist <= settings.radius;

				if (character != NULL 
					&& !character->IsDead() 
					&& settings.affectedTeam == AOEEnums::Ally ? character->team == settings.owner->team : character->team != settings.owner->team
					&& inRange) {

					damage.healthChange = tempDamage;
					damage.target = character;
					character->ChangeHealth(damage);
				}
			}
		}

		settings.world->GetTimerManager().SetTimer(TimerHandle_AOERate, this, &UAOEHealthChange::Activate, settings.rate);
	}
}
