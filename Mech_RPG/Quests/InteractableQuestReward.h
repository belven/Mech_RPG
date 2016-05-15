// Copyright of Explosive Industries

#pragma once

#include "QuestReward.h"
#include "InteractableQuestReward.generated.h"

class AInteractable;
UCLASS()
class MECH_RPG_API UInteractableQuestReward : public UQuestReward
{
	GENERATED_BODY()

private:

	UPROPERTY()
	AInteractable* interactable;
public:

	UFUNCTION(BlueprintCallable, Category = "Quest")
		virtual	void GiveReward(AMech_RPGCharacter* questOwner);

	UFUNCTION(BlueprintCallable, Category = "Quest")
		static UInteractableQuestReward* CreateInteractableQuestReward(AInteractable* inInteractable, UQuest* inQuestChain = nullptr);

	UFUNCTION(BlueprintCallable, Category = "Quest")
		AInteractable* GetInteractable() {
		return interactable;
	}

	UFUNCTION(BlueprintCallable, Category = "Quest")
		void SetInteractable(AInteractable* val) {
		interactable = val;
	}
};
