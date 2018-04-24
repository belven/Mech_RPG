// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "Items/Item.h"
#include "ItemQuestReward.h"
#include "Characters/Mech_RPGCharacter.h"

void UItemQuestReward::GiveReward(AMech_RPGCharacter* questOwner)
{
	for (UItem* item : rewards) {
		questOwner->AddItem(item);
	}

	Super::GiveReward(questOwner);
}

UItemQuestReward* UItemQuestReward::CreateItemQuestReward(TArray<UItem*> inRewards, UQuest* inQuestChain)
{
	UItemQuestReward* reward = NewObject<UItemQuestReward>(StaticClass());
	reward->SetRewards(inRewards);
	reward->SetQuestToGive(inQuestChain);
	return reward;
}
