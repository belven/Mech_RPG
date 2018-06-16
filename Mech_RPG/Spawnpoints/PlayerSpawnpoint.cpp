// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "PlayerSpawnpoint.h"
#include "Characters/Mech_RPGCharacter.h"

APlayerSpawnpoint::APlayerSpawnpoint() :
	defaultSpawnpoint(false), Super()
{

}

void APlayerSpawnpoint::BeginPlay() {
	if (GetWorld() != nullptr && defaultSpawnpoint) {
		UGroup* group = UMiscLibrary::GetPlayerGroup();
		group->SetID(team);

		AMech_RPGCharacter* character = nullptr;
		bool healerSpawned = false;
		FVector loc;

		character = SpawnCharacter(playerClass, 400);

		if (character != nullptr) {
			SetUpCharacter(character, group, playerRole);
		}

		for (ERole role : allyRoles) {
			character = SpawnCharacter(classToSpawn, 400);
			if (character != nullptr) {
				SetUpCharacter(character, group, role);
			}
		}
	}
}