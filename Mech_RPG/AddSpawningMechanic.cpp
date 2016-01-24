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

	if (GetBoss() != nullptr) {
		if (trigger == nullptr) {
			trigger = UDamageSpawnTrigger::CreateDamageSpawnTrigger(GetBoss(), spawnAmount, 0.15);
		}

		trigger->OnSpawnTrigger.AddUniqueDynamic(this, &AAddSpawningMechanic::TriggerSpawn);
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
		AMech_RPGCharacter* character = GetWorld()->SpawnActor<AMech_RPGCharacter>(classToSpawn, nav.Location, GetActorRotation());
		character != nullptr ? character->SpawnDefaultController() : true;
		character->CreatePresetRole((GroupEnums::Role)UMiscLibrary::GetRandomEnum(GroupEnums::End));
		spawnAmount--;
	}
}

void AAddSpawningMechanic::SetUpCharacter(AMech_RPGCharacter* character) {
}