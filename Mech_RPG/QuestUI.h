// Copyright of Explosive Industries

#pragma once

#include "Blueprint/UserWidget.h"
#include "QuestUI.generated.h"

class UQuest;
class UQuestObjective;

UCLASS()
class MECH_RPG_API UQuestUI : public UUserWidget
{
	GENERATED_BODY()

private:

	UPROPERTY()
		UQuest* quest;
public:
	UFUNCTION(BlueprintCallable, Category = "Quest")
		UQuest* GetQuest();

	UFUNCTION(BlueprintCallable, Category = "Quest")
		void SetQuest(UQuest* inQuest);

	UFUNCTION(BlueprintCallable, Category = "Quest")
		TArray<UQuestObjective*>& GetObjectives();

	UFUNCTION(BlueprintCallable, Category = "Quest")
		FString GetQuestName();
};
