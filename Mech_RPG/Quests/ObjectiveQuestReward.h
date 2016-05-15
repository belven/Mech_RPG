// Copyright of Explosive Industries

#pragma once

#include "QuestReward.h"
#include "ObjectiveQuestReward.generated.h"

class UQuestObjective;
UCLASS()
class MECH_RPG_API UObjectiveQuestReward : public UQuestReward
{
	GENERATED_BODY()

private:

	UPROPERTY()
	UQuestObjective* objective;

public:
	UFUNCTION(BlueprintCallable, Category = "Quest")
		virtual	void GiveReward(AMech_RPGCharacter* questOwner);

	UFUNCTION(BlueprintCallable, Category = "Quest")
		static UObjectiveQuestReward* CreateObjectiveQuestReward(UQuestObjective* inObjective, UQuest* inQuestChain = nullptr);

	UFUNCTION(BlueprintCallable, Category = "Quest")
		UQuestObjective* GetObjective() {
		return objective;
	}

	UFUNCTION(BlueprintCallable, Category = "Quest")
		void SetObjective(UQuestObjective* val) {
		objective = val;
	}
};
