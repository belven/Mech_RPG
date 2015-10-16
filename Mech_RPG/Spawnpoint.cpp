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
	FNavLocation nav;
	bool healerSpawned = false;

	for (int i = 0; i < spawnAmount; i++) {
		if (GetWorld() != NULL) {
			GetWorld()->GetNavigationSystem()->GetRandomPointInNavigableRadius(GetActorLocation(), 400, nav);
			//nav.Location.Y = GetActorLocation().Y;
			AMech_RPGCharacter* character = UMiscLibrary::SpawnCharacter<AMech_RPGCharacter>(GetWorld(), nav.Location, GetActorRotation(), classToSpawn);
			
			GroupEnums::Role role = UGroup::GetRandomRole();

			if (role == GroupEnums::Healer) {
				if (!healerSpawned)
					healerSpawned = true;
				
				while (role == GroupEnums::Healer) {
					role = UGroup::GetRandomRole();
				}
			}

			character->CreatePresetRole();
		}
	}
}