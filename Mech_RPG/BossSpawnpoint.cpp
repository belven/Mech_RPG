// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "BossSpawnpoint.h"
#include "Boss.h"
#include "HealMechanic.h"
#include "DamageBoostMechanic.h"

ABossSpawnpoint::ABossSpawnpoint() {
	static ConstructorHelpers::FClassFinder<ABoss> BossClass(TEXT("/Game/TopDown/Blueprints/Bosses/Basic_Boss"));
	if (BossClass.Succeeded()) {
		bossClass = BossClass.Class;
	}

	static ConstructorHelpers::FClassFinder<ABossMechanic> MechanicClass(TEXT("/Game/TopDown/Blueprints/Bosses/Boss_Mehanics/Damage_Mechanic"));
	if (MechanicClass.Succeeded()) {
		mechanicClass = MechanicClass.Class;
	}
}

void ABossSpawnpoint::BeginPlay() {
	FNavLocation nav;
	FVector loc = GetActorLocation();
	GroupEnums::Role role = UGroup::GetRandomRole();
	BossEnums::BossRole bossRole = ABoss::GetRandomRole();

	bool healerSpawned = false;

	ABoss* character = UMiscLibrary::SpawnCharacter<ABoss>(GetWorld(), GetActorLocation(), GetActorRotation(), bossClass);

	if (bossRole == GroupEnums::Healer) {
		if (!healerSpawned)
			healerSpawned = true;
	}

	loc = character->GetActorLocation();
	loc.Z += (character->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight());
	character->SetActorLocation(loc);
	
	character->CreatePresetRole(bossRole);

	for (int i = 0; i < amountOfMechanics; i++) {
		GetWorld()->GetNavigationSystem()->GetRandomPointInNavigableRadius(GetActorLocation(), 400, nav);
		ABossMechanic* character = UMiscLibrary::SpawnCharacter<ABossMechanic>(GetWorld(), nav.Location, GetActorRotation(), mechanicClass);
		loc = character->GetActorLocation();
		
		loc.Z += (character->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight());
		character->SetActorLocation(loc);

		if (role == GroupEnums::Healer) {
			if (!healerSpawned) {
				healerSpawned = true;
			}

			while (role == GroupEnums::Healer) {
				role = UGroup::GetRandomRole();
			}
		}
		character->CreatePresetRole(role);
	}

	if (spawnAdds) {
		Super::BeginPlay();
	}
}
