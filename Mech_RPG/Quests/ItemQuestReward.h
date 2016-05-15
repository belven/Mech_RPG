// Copyright of Explosive Industries

#pragma once

#include "QuestReward.h"
#include "ItemQuestReward.generated.h"


class AItem;

UCLASS()
class MECH_RPG_API UItemQuestReward : public UQuestReward
{
	GENERATED_BODY()
private:

	TArray<AItem*> rewards;

public:
	UFUNCTION(BlueprintCallable, Category = "Quest")
		virtual	void GiveReward(AMech_RPGCharacter* questOwner);

	UFUNCTION(BlueprintCallable, Category = "Quest")
	static UItemQuestReward* CreateItemQuestReward(TArray<AItem*> inRewards, UQuest* inQuestChain = nullptr);
		
	UFUNCTION(BlueprintCallable, Category = "Quest")
		TArray<AItem*> GetRewards() {
		return rewards;
	}

	UFUNCTION(BlueprintCallable, Category = "Quest")
		void SetRewards(TArray<AItem*> newVal) {
		rewards = newVal;
	}

};
