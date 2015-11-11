// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "DamageSpawnTrigger.h"

UDamageSpawnTrigger* UDamageSpawnTrigger::CreateDamageSpawnTrigger(ABoss* inBoss, int inAmount, float inPercent) {
	UDamageSpawnTrigger* tempTrigger = NewObject<UDamageSpawnTrigger>(StaticClass());
	if (inBoss != NULL) {
		tempTrigger->SetBoss(inBoss);
		inBoss->OnHealthChange.AddUniqueDynamic(tempTrigger, &UDamageSpawnTrigger::BossHealthChange);
		tempTrigger->lastHealthPecent = inBoss->GetHealth() / inBoss->GetMaxHealth();
	}
	else {
		tempTrigger->lastHealthPecent = 1;
	}

	tempTrigger->SetAmount(inAmount);
	tempTrigger->percent = inPercent;
	return tempTrigger;
}

void UDamageSpawnTrigger::BossHealthChange(FHealthChange damage) {
	if (GetAmount() > 0) {
		float healthPercent = GetBoss()->GetHealth() / GetBoss()->GetMaxHealth();
		if (lastHealthPecent - healthPercent >= percent) {
			lastHealthPecent = healthPercent;
			amount--;

			// Spawn adds
			if (OnSpawnTrigger.IsBound()) {
				OnSpawnTrigger.Broadcast();
			}

			if (amount == 0) {
				GetBoss()->OnHealthChange.RemoveDynamic(this, &UDamageSpawnTrigger::BossHealthChange);
			}
		}
	}
}