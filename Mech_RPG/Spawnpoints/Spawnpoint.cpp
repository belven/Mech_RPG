// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "Spawnpoint.h"
#include "Characters/Mech_RPGCharacter.h"

#define mGetRandomPointInNavigableRadius(spawnRadius, nav) GetWorld()->GetNavigationSystem()->GetRandomPointInNavigableRadius(GetActorLocation(), spawnRadius, nav);
#define mFindNavLocation(spawnRadius) UMiscLibrary::FindNavLocation(this, spawnRadius)
#define mSpawnCharacter UMiscLibrary::SpawnCharacter<AMech_RPGCharacter>

TMap<UClass*, TArray<ASpawnpoint*>> ASpawnpoint::spawnPoints;
float ASpawnpoint::defaultSpawnRadius = 200;

ASpawnpoint::ASpawnpoint()
{
	static ConstructorHelpers::FClassFinder<AMech_RPGCharacter> AIClass(TEXT("/Game/TopDown/Blueprints/AI/AI"));
	if (AIClass.Succeeded())
	{
		classToSpawn = AIClass.Class;
	}
}

AMech_RPGCharacter* ASpawnpoint::SpawnCharacter(TSubclassOf<class AMech_RPGCharacter> spawnClass, int spawnRadius)
{
	FNavLocation nav;
	AMech_RPGCharacter* character = nullptr;
	int count = 0;

	while (count < 10 && character == nullptr)
	{
		character = mSpawnCharacter(GetWorld(), mFindNavLocation(spawnRadius), GetActorRotation(), spawnClass);
		count++;
	}

	return character;
}

void ASpawnpoint::SetUpCharacter(AMech_RPGCharacter* character, UGroup* group, ERole role)
{
	character->SetTeam(team);
	AdjustCharacterLocationByCapsule(character);
	character->SetGroup(group);
	character->CreatePresetRole(role, 1 + bonusGrade, bonusQuality);
}

void ASpawnpoint::AdjustCharacterLocationByCapsule(AMech_RPGCharacter* character)
{
	FVector loc;
	loc = character->GetActorLocation();
	loc.Z += (character->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
	character->SetActorLocation(loc);
}

void ASpawnpoint::BeginPlay()
{
	Super::BeginPlay();
	bool healerSpawned = false;
	UGroup* group = UGroup::CreateGroup(team);

	for (int i = 0; i < spawnAmount; i++)
	{
		if (GetWorld() != nullptr)
		{
			AMech_RPGCharacter* character = SpawnCharacter(classToSpawn, ASpawnpoint::defaultSpawnRadius);
			ERole role = UGroup::GetRandomRole();

			if (character != nullptr)
			{
				if (role == ERole::Healer)
				{
					if (!healerSpawned)
					{
						healerSpawned = true;
					}
					else
					{
						while (role == ERole::Healer)
						{
							role = UGroup::GetRandomRole();
						}
					}
				}

				SetUpCharacter(character, group, role);
			}
		}
	}
}

TArray<ASpawnpoint*>* ASpawnpoint::GetSpawnpoints(UClass* classToFind)
{
	return spawnPoints.Find(classToFind);
}
