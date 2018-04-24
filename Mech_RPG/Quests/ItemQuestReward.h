// Copyright of Explosive Industries

#pragma once

#include "QuestReward.h"
#include "ItemQuestReward.generated.h"


class UItem;

UCLASS()
class MECH_RPG_API UItemQuestReward : public UQuestReward
{
	GENERATED_BODY()
private:
	
	UPROPERTY()
		TArray<UItem*> rewards;

public:
	UFUNCTION(BlueprintCallable, Category = "Quest")
		virtual	void GiveReward(AMech_RPGCharacter* questOwner);

	UFUNCTION(BlueprintCallable, Category = "Quest")
	static UItemQuestReward* CreateItemQuestReward(TArray<UItem*> inRewards, UQuest* inQuestChain = nullptr);
		
	UFUNCTION(BlueprintCallable, Category = "Quest")
		TArray<UItem*> GetRewards() {
		return rewards;
	}

	UFUNCTION(BlueprintCallable, Category = "Quest")
		void SetRewards(TArray<UItem*> newVal) {
		rewards = newVal;
	}

};
