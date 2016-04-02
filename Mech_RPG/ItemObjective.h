// Copyright of Explosive Industries

#pragma once

#include "QuestObjective.h"
#include "ItemObjective.generated.h"

class AItem;

UCLASS()
class MECH_RPG_API UItemObjective : public UQuestObjective
{
	GENERATED_BODY()
	
private:
	AItem* item;
	bool complete = false;
	
public:
	UFUNCTION(BlueprintCallable, Category = "Interactive Quest Objective")
		virtual void SetUpListeners(UQuest* quest) override;

	UFUNCTION(BlueprintCallable, Category = "Interactive Quest Objective")
		virtual bool IsComplete() override;

	UFUNCTION(BlueprintCallable, Category = "Interactive Quest Objective")
		virtual FString GetObjectiveText();

	UFUNCTION(BlueprintCallable, Category = "Interactive Quest Objective")
		void PlayerItemPickup(AItem* inItem);

	UFUNCTION(BlueprintCallable, Category = "Interactive Quest Objective")
		static UItemObjective* CreateItemObjective(AItem* inItem);
};
