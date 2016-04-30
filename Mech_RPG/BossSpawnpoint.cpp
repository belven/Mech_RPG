// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "BossSpawnpoint.h"
#include "Boss.h"
#include "DamageBoostMechanic.h"

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
	GroupEnums::Role role = UGroup::GetRandomRole();
	GroupEnums::Role bossRole = ABoss::GetRandomRole();
	UGroup* group = UGroup::CreateGroup(team);

	bool healerSpawned = false;

	ABoss* character = dynamic_cast<ABoss*>(SpawnCharacter(bossClass, 400));

	if (bossRole == GroupEnums::Healer) {
		if (!healerSpawned) {
			healerSpawned = true;
		}
	}

	SetUpCharacter(character, group, bossRole);

	loop(i, amountOfMechanics) {
		ABossMechanic* character = dynamic_cast<ABossMechanic*>(SpawnCharacter(mechanicClass, 400)) ;

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

		SetUpCharacter(Cast<AMech_RPGCharacter>(character), group, role);
	}

	if (spawnAdds) {
		Super::BeginPlay();
	}
}
