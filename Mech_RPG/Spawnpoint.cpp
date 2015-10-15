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

	for (int i = 0; i < spawnAmount; i++) {
		GetWorld()->GetNavigationSystem()->GetRandomPointInNavigableRadius(GetActorLocation(), 400, nav);
		AMech_RPGCharacter* character = UMiscLibrary::SpawnCharacter(GetWorld(), nav.Location, GetActorRotation(), classToSpawn);
		character->CreatePresetRole(UGroup::GetRandomRole());
	}
}