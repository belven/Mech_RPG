// Fill out your copyright notice in the Description page of Project Settings.

#include "Mech_RPG.h"
#include "TestAOE_Temp.h"
#include "Mech_RPGCharacter.h"

UTestAOE_Temp* UTestAOE_Temp::CreateTestAOE(FTempAOESettings inSettings) {
	UTestAOE_Temp* tempAOE = NewObject<UTestAOE_Temp>(UTestAOE_Temp::StaticClass());
	tempAOE->settings = inSettings;
	tempAOE->Activate();
	return tempAOE;
}

void  UTestAOE_Temp::Activate() {
	if (timesRan < settings.duration) {
		timesRan++;

		FDamage damage;
		FVector locationToUse = settings.usesTarget && settings.target != NULL ? settings.target->GetActorLocation() : settings.location;

		damage.damager = settings.owner;

		for (FConstPawnIterator iter = settings.world->GetPawnIterator(); iter; iter++) {
			APawn* pawn = iter->Get();

			if (iter->IsValid() && pawn != NULL && pawn->GetClass()->IsChildOf(AMech_RPGCharacter::StaticClass())) {
				AMech_RPGCharacter* character = Cast<AMech_RPGCharacter>(pawn);
				float dist = (locationToUse - character->GetActorLocation()).Size();

				if (dist < 0) {
					dist = -dist;
				}

				bool inRange = dist <= settings.radius;

				if (character != NULL && !character->IsDead() && character->startingGroupID == settings.affectedTeam && inRange) {
					//UE_LOG(LogTemp, Log, TEXT("Dist = %f"), dist);
					damage.damagedDealt = character->GetMaxHealth() * settings.healthChange;
					damage.target = character;
					character->Hit(damage);
				}
			}
		}

		settings.world->GetTimerManager().SetTimer(TimerHandle_AOERate, this, &UTestAOE_Temp::Activate, settings.rate);
	}
}
