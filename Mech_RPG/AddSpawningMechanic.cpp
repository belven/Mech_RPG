// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "AddSpawningMechanic.h"


AAddSpawningMechanic::AAddSpawningMechanic() : Super() {
	static ConstructorHelpers::FClassFinder<AMech_RPGCharacter> AIClass(TEXT("/Game/TopDown/Blueprints/AI/AI"));
	if (AIClass.Succeeded()) {
		classToSpawn = AIClass.Class;
	}
}

void AAddSpawningMechanic::Tick(float DeltaTime) {
	AMech_RPGCharacter::Tick(DeltaTime);

	if (!IsDead()) {
		if (GetBoss() && !GetBoss()->IsDead() && GetBoss()->GetHealth() < GetBoss()->GetMaxHealth()) {
			//GetBoss()->SetHealth(GetBoss()->GetHealth() + healAmount * DeltaTime);
		}
	}
	else {
		//Destroy();
	}
}

void AAddSpawningMechanic::BeginPlay() {
	Super::BeginPlay();

	if (trigger != NULL) {
		trigger->OnSpawnTrigger.AddDynamic(this, &AAddSpawningMechanic::TriggerSpawn);
		trigger->SetBoss(GetBoss());
		trigger->SetAmount(spawnAmount);
	}
}

void AAddSpawningMechanic::TriggerSpawn() {
	if (classToSpawn != NULL) {
		AMech_RPGCharacter* character = GetWorld()->SpawnActor<AMech_RPGCharacter>(classToSpawn);
		FNavLocation nav;
		GetWorld()->GetNavigationSystem()->GetRandomPointInNavigableRadius(GetActorLocation(), 200, nav);
		nav.Location.Z = GetActorLocation().Z;
		character->SetActorLocation(nav.Location);
		character->OnPostBeginPlay.AddDynamic(this, &AAddSpawningMechanic::SetUpCharacter);

		spawnAmount--;
	}
}

void AAddSpawningMechanic::SetUpCharacter(AMech_RPGCharacter* character) {
	character->CreatePresetRole(GroupEnums::DPS);
	character->SetDefenceModifier(0.5);
	character->SetDamageModifier(3);
}