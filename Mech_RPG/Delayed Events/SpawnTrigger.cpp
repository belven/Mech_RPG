// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "SpawnTrigger.h"


USpawnTrigger* USpawnTrigger::CreateSpawnTrigger(ABoss* inBoss) {
	USpawnTrigger* tempTrigger = NewObject<USpawnTrigger>(StaticClass());
	tempTrigger->SetBoss(inBoss);
	return tempTrigger;
}

void USpawnTrigger::SetBoss(ABoss* inBoss) {
	boss = inBoss;
}

ABoss* USpawnTrigger::GetBoss() {
	return boss;
}

int32 USpawnTrigger::GetAmount() {
	return amount;
}

void USpawnTrigger::SetAmount(int32 inAmount) {
	amount = inAmount;
}