// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "QuestReward.h"
#include "Item.h"
#include "Mech_RPGCharacter.h"

void UQuestReward::GiveReward(AMech_RPGCharacter* questOwner) {

}


UQuestReward* UQuestReward::CreateQuestReward() {
	UQuestReward* tempQuestReward = NewObject<UQuestReward>(UQuestReward::StaticClass());
	return tempQuestReward;
}


TArray<AItem*> UQuestReward::GetRewards() {
	return rewards;
}


void UQuestReward::SetRewards(TArray<AItem*> newVal) {
	rewards = newVal;
}



