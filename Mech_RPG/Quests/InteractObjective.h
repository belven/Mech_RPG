// Copyright of Explosive Industries

#pragma once

#include "QuestObjective.h"
#include "InteractObjective.generated.h"

/**
 *
 */
UCLASS()
class MECH_RPG_API UInteractObjective : public UQuestObjective
{
	GENERATED_BODY()


private:
	UPROPERTY()
		AInteractable* interactable;
	UPROPERTY()
		bool complete = false;

public:
	UFUNCTION(BlueprintCallable, Category = "Interactive Quest Objective")
		virtual void SetUpListeners(UQuest* inQuest) override;

	UFUNCTION(BlueprintCallable, Category = "Interactive Quest Objective")
		virtual bool IsComplete() override;

	UFUNCTION(BlueprintCallable, Category = "Interactive Quest Objective")
		virtual FString GetObjectiveText() override;

	UFUNCTION(BlueprintCallable, Category = "Interactive Quest Objective")
		void PlayerInteraction(AInteractable* inInteractable);

	UFUNCTION(BlueprintCallable, Category = "Interactive Quest Objective")
		static UInteractObjective* CreateInterractObjective(FString name, AInteractable* inInteractable);

};
