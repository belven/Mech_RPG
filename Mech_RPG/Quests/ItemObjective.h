// Copyright of Explosive Industries

#pragma once

#include "QuestObjective.h"
#include "ItemObjective.generated.h"

class UItem;

UCLASS()
class MECH_RPG_API UItemObjective : public UQuestObjective
{
	GENERATED_BODY()

private:
	UPROPERTY()
		UItem* item;

public:
	UFUNCTION(BlueprintCallable, Category = "Interactive Quest Objective")
		virtual void SetUpListeners(UQuest* inQuest) override;

	UFUNCTION(BlueprintCallable, Category = "Interactive Quest Objective")
		virtual bool IsComplete() override;

	UFUNCTION(BlueprintCallable, Category = "Interactive Quest Objective")
		virtual FString GetObjectiveText();

	UFUNCTION(BlueprintCallable, Category = "Interactive Quest Objective")
		void PlayerItemPickup(UItem* inItem);

	UFUNCTION(BlueprintCallable, Category = "Interactive Quest Objective")
		static UItemObjective* CreateItemObjective(FString name, UItem* inItem);
};
