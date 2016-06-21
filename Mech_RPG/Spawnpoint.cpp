// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "Spawnpoint.h"
#include "Mech_RPGCharacter.h"


ASpawnpoint::ASpawnpoint() {
	static ConstructorHelpers::FClassFinder<AMech_RPGCharacter> AIClass(TEXT("/Game/TopDown/Blueprints/AI/AI"));
	if (AIClass.Succeeded()) {
		classToSpawn = AIClass.Class;
	}
}

AMech_RPGCharacter* ASpawnpoint::SpawnCharacter(TSubclassOf<class AMech_RPGCharacter> spawnClass, int spawnRadius) {
	FNavLocation nav;
	AMech_RPGCharacter* character = nullptr;
	int count = 0;

	while (count < 10
		&& (character == nullptr || !UMiscLibrary::CanSee(GetWorld(), GetActorLocation(), character->GetActorLocation()))) {
		if (character != nullptr) {
			GetWorld()->GetNavigationSystem()->GetRandomPointInNavigableRadius(GetActorLocation(), spawnRadius, nav);
			character->SetActorLocation(nav.Location);
			count++;
		}
		else {
			GetWorld()->GetNavigationSystem()->GetRandomPointInNavigableRadius(GetActorLocation(), spawnRadius, nav);
			character = UMiscLibrary::SpawnCharacter<AMech_RPGCharacter>(GetWorld(), nav.Location, GetActorRotation(), spawnClass);
			count++;
		}
	}
	return character;
}

void ASpawnpoint::SetUpCharacter(AMech_RPGCharacter* character, UGroup* group, GroupEnums::Role role) {
	FVector loc;
	character->SetTeam(team);
	loc = character->GetActorLocation();
	loc.Z += (character->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
	character->SetActorLocation(loc);
	character->SetGroup(group);
	character->CreatePresetRole(role);
}

void ASpawnpoint::BeginPlay() {
	Super::BeginPlay();
	bool healerSpawned = false;
	UGroup* group = UGroup::CreateGroup(team);

	for (int i = 0; i < spawnAmount; i++) {
		if (GetWorld() != nullptr) {
			AMech_RPGCharacter* character = SpawnCharacter(classToSpawn, 400);
			GroupEnums::Role role = UGroup::GetRandomRole();

			if (character != nullptr) {
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

				SetUpCharacter(character, group, role);
			}
		}
	}
}