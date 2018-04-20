// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "DamageSpawnTrigger.h"

UDamageSpawnTrigger* UDamageSpawnTrigger::CreateDamageSpawnTrigger(ABoss* inBoss, int inAmount, float inPercent) {
	UDamageSpawnTrigger* tempTrigger = NewObject<UDamageSpawnTrigger>(StaticClass());
	tempTrigger->SetBoss(inBoss);
	tempTrigger->SetAmount(inAmount);
	tempTrigger->percent = inPercent;
	return tempTrigger;
}

void UDamageSpawnTrigger::SetBoss(ABoss* inBoss) {
	Super::SetBoss(inBoss);
	if (inBoss != nullptr) {
		inBoss->OnPostHealthChange.AddUniqueDynamic(this, &UDamageSpawnTrigger::BossHealthChange);
	}
}

void UDamageSpawnTrigger::BossHealthChange(FHealthChange& damage) {
	if (GetBoss() != nullptr && GetAmount() > 0 && !damage.heals) {
		// Convert damage taken to a percent of the bosses maxHealth
		float healthPercent = damage.changeAmount / GetBoss()->GetMaxHealth();
		percentDamageTaken += healthPercent;

		while (percentDamageTaken >= percent && GetAmount() > 0) {
			percentDamageTaken -= percent;

			// Spawn adds
			if (OnTrigger.IsBound()) {
				OnTrigger.Broadcast();
				amount--;
			}
		}

		if (amount == 0) {
			GetBoss()->OnPostHealthChange.RemoveDynamic(this, &UDamageSpawnTrigger::BossHealthChange);
		}
	}
}