// Fill out your copyright notice in the Description page of Project Settings.

#include "Mech_RPG.h"
#include "AOEVolume.h"
#include "Characters/Mech_RPGCharacter.h"
#include "Components/BrushComponent.h"
#include "Engine/Brush.h"

void AAOEVolume::Tick(float DeltaTime) {
	if (currentDuration < maxDuration) {
		currentDuration += DeltaTime;

		for (FConstPawnIterator iter = GetWorld()->GetPawnIterator(); iter; iter++) {
			APawn* pawn = iter->Get();


			if (pawn != NULL && pawn->GetClass()->IsChildOf(AMech_RPGCharacter::StaticClass()) && pawn->GetDistanceTo(this) <= 200) {
				AMech_RPGCharacter* character = Cast<AMech_RPGCharacter>(pawn);

				if (character != NULL 
					&& !character->IsDead() 
					&& character->GetTeam() == affectedTeam) {
					FHealthChange damage;
					damage.changeAmount = healthChange * DeltaTime;
					damage.manipulator = owner;
					damage.target = character;
					character->ChangeHealth(damage);
				}
			}
		}
	}
	else {
		//Destroy();
	}

		//TArray<AActor*> actors;

		//GetOverlappingActors(actors, AMech_RPGCharacter::StaticClass());

		//if (actors.Num() > 0) {
		//	for (AActor* actor : actors) {
		//		AMech_RPGCharacter* character = Cast<AMech_RPGCharacter>(actor);

		//		if (character != NULL && !character->IsDead() && character->startingGroupID == affectedTeam) {
		//			FHealthChange damage;
		//			damage.damagedDealt = healthChange;
		//			damage.damager = owner;
		//			damage.target = character;
		//			character->ChangeHealth(damage);
		//		}
		//	}
		//}
	
}

AAOEVolume* AAOEVolume::CreateAAOEVolume(FAOESettings settings) {
	AAOEVolume* tempAOE = settings.world->SpawnActor<AAOEVolume>(settings.Location, settings.Location.Rotation());
	tempAOE->healthChange = settings.inHealthChange;
	tempAOE->affectedTeam = settings.inAffectedTeam;
	tempAOE->maxDuration = settings.duration;
	tempAOE->owner = settings.owner;
	tempAOE->SetActorHiddenInGame(false);
	tempAOE->SetActorTickEnabled(true);
	//tempAOE->Brush = settings.world->SpawnActor<UModel>(settings.Location, settings.Location.Rotation());
	//tempAOE->Brush->Bounds.SphereRadius = 200;
	return tempAOE;
}
