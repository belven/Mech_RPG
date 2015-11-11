// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "AddSpawningMechanic.h"
#include "DamageSpawnTrigger.h"


AAddSpawningMechanic::AAddSpawningMechanic() : Super() {
	static ConstructorHelpers::FClassFinder<AMech_RPGCharacter> AIClass(TEXT("/Game/TopDown/Blueprints/AI/AI"));
	if (AIClass.Succeeded()) {
		classToSpawn = AIClass.Class;
	}
}

void AAddSpawningMechanic::Tick(float DeltaTime) {
	AMech_RPGCharacter::Tick(DeltaTime);

	if (IsDead()) {
		//Destroy();
	}
}

void AAddSpawningMechanic::BeginPlay() {
	Super::BeginPlay();

	if (GetBoss() != NULL) {
		if (trigger != NULL) {
			trigger->OnSpawnTrigger.AddUniqueDynamic(this, &AAddSpawningMechanic::TriggerSpawn);
			trigger->SetBoss(GetBoss());
			trigger->SetAmount(spawnAmount);
		}
		else {
			trigger = UDamageSpawnTrigger::CreateDamageSpawnTrigger(GetBoss(), 5, 0.1F);
			trigger->OnSpawnTrigger.AddUniqueDynamic(this, &AAddSpawningMechanic::TriggerSpawn);
		}
	}
}

void AAddSpawningMechanic::TriggerSpawn() {
	if (classToSpawn != NULL && !IsDead()) {
		FNavLocation nav;
		GetWorld()->GetNavigationSystem()->GetRandomPointInNavigableRadius(GetActorLocation(), 400, nav);
		nav.Location.Z = GetActorLocation().Z;
		AMech_RPGCharacter* character = GetWorld()->SpawnActor<AMech_RPGCharacter>(classToSpawn, nav.Location, GetActorRotation());

		SetUpCharacter(character);
		spawnAmount--;
	}
}

void AAddSpawningMechanic::SetUpCharacter(AMech_RPGCharacter* character) {
	character->SpawnDefaultController();
}