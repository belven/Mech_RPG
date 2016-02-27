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
	if (inBoss != NULL) {
		inBoss->OnPostHealthChange.AddUniqueDynamic(this, &UDamageSpawnTrigger::BossHealthChange);
		lastHealthPecent = inBoss->GetHealth() / inBoss->GetMaxHealth();
	}
}

void UDamageSpawnTrigger::BossHealthChange(FHealthChange damage) {
	if (GetBoss() != NULL && GetAmount() > 0) {
		float healthPercent = GetBoss()->GetHealth() / GetBoss()->GetMaxHealth();
		if (lastHealthPecent - healthPercent >= percent) {
			lastHealthPecent = healthPercent;
			amount--;

			// Spawn adds
			if (OnTrigger.IsBound()) {
				OnTrigger.Broadcast();
			}

			if (amount == 0) {
				GetBoss()->OnPostHealthChange.RemoveDynamic(this, &UDamageSpawnTrigger::BossHealthChange);
			}
		}
	}
}