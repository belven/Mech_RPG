// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "Spawnpoint.h"


ASpawnpoint::ASpawnpoint() {
	static ConstructorHelpers::FClassFinder<AMech_RPGCharacter> AIClass(TEXT("/Game/TopDown/Blueprints/AI/AI"));
	if (AIClass.Succeeded()) {
		classToSpawn = AIClass.Class;
	}
}

void ASpawnpoint::BeginPlay() {
	Super::BeginPlay();
	FVector loc;
	FNavLocation nav;
	bool healerSpawned = false;

	for (int i = 0; i < spawnAmount; i++) {
		if (GetWorld() != NULL) {
			AMech_RPGCharacter* character = NULL;
			int count = 0;

			while (character == NULL && count < 10) {
				GetWorld()->GetNavigationSystem()->GetRandomPointInNavigableRadius(GetActorLocation(), 400, nav);
				character = UMiscLibrary::SpawnCharacter<AMech_RPGCharacter>(GetWorld(), nav.Location, GetActorRotation(), classToSpawn);
				count++;
			}

			if (character != NULL) {
				loc = character->GetActorLocation();
				loc.Z += (character->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight());
				character->SetActorLocation(loc);

				GroupEnums::Role role = UGroup::GetRandomRole();

				if (role == GroupEnums::Healer) {
					if (!healerSpawned) {
						healerSpawned = true;
					}
					else {
						while (role == GroupEnums::Healer) {
							role = UGroup::GetRandomRole();
						}
					}
				}

				character->CreatePresetRole(role);
			}
		}
	}
}