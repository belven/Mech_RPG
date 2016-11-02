// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "AddSpawningMechanic.h"
#include "Delayed Events/DamageSpawnTrigger.h"


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

	if (GetBoss() != nullptr) {
		if (trigger == nullptr) {
			trigger = UDamageSpawnTrigger::CreateDamageSpawnTrigger(GetBoss(), spawnAmount, 0.15);
		}

		trigger->OnTrigger.AddUniqueDynamic(this, &AAddSpawningMechanic::TriggerSpawn);
		trigger->SetBoss(GetBoss());
		trigger->SetAmount(spawnAmount); 
	}
}

void AAddSpawningMechanic::SetBoss(ABoss* newVal) {
	Super::SetBoss(newVal);
}

void AAddSpawningMechanic::TriggerSpawn() {
	if (classToSpawn != nullptr && !IsDead()) {
		FNavLocation nav;
		GetWorld()->GetNavigationSystem()->GetRandomPointInNavigableRadius(GetActorLocation(), 400, nav);
		nav.Location.Z = GetActorLocation().Z;
		AMech_RPGCharacter* character = UMiscLibrary::SpawnCharacter<AMech_RPGCharacter>(GetWorld(), nav.Location, GetActorRotation(), classToSpawn);
		if (character != nullptr) {
			character->SpawnDefaultController();
			character->CreatePresetRole((GroupEnums::Role)UMiscLibrary::GetRandomEnum(GroupEnums::End));
			character->SetGroup(GetGroup());
			character->SetTeam(GetTeam());
			spawnAmount--;
		}
	}
}

void AAddSpawningMechanic::SetUpCharacter(AMech_RPGCharacter* character) {
}