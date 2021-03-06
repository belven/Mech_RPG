// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "BossSpawnpoint.h"
#include "Characters/Boss.h"
#include "Characters/DamageBoostMechanic.h"

#define loop(i, max) for(i = 0; i < max; i++)

ABossSpawnpoint::ABossSpawnpoint() {
	static ConstructorHelpers::FClassFinder<ABoss> BossClass(TEXT("/Game/TopDown/Blueprints/Bosses/Boss"));
	if (BossClass.Succeeded()) {
		bossClass = BossClass.Class;
	}
	else {
		bossClass = ABoss::StaticClass();
	}

	static ConstructorHelpers::FClassFinder<ABossMechanic> MechanicClass(TEXT("/Game/TopDown/Blueprints/Bosses/Boss_Mehanics/Boss_Damage_Boost"));
	if (MechanicClass.Succeeded()) {
		mechanicClass = MechanicClass.Class;
	}
	else {
		mechanicClass = ADamageBoostMechanic::StaticClass();
	}
}

void ABossSpawnpoint::BeginPlay() {
	int i;
	ERole role = UGroup::GetRandomRole();
	ERole bossRole = ABoss::GetRandomRole();
	UGroup* group = UGroup::CreateGroup(team);

	bool healerSpawned = false;

	ABoss* boss = dynamic_cast<ABoss*>(SpawnCharacter(bossClass, 400));

	if (bossRole == ERole::Healer) {
		if (!healerSpawned) {
			healerSpawned = true;
		}
	}

	SetUpCharacter(boss, group, bossRole);

	loop(i, amountOfMechanics) {
		ABossMechanic* bossMechanic = dynamic_cast<ABossMechanic*>(SpawnCharacter(mechanicClass, 400)) ;

		if (bossMechanic != nullptr) {
			if (role == ERole::Healer) {
				if (!healerSpawned) {
					healerSpawned = true;
				}
				else {
					while (role == ERole::Healer) {
						role = UGroup::GetRandomRole();
					}
				}
			}

			SetUpCharacter(Cast<AMech_RPGCharacter>(bossMechanic), group, role);
		}
	}

	if (spawnAdds) {
		Super::BeginPlay();
	}
}
