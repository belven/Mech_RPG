// Copyright of Explosive Industries

#pragma once

#include "Blueprint/UserWidget.h"
#include "QuestObjectiveUI.generated.h"

class UQuestObjective;

UCLASS()
class MECH_RPG_API UQuestObjectiveUI : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY()
		UQuestObjective* objective;
public:
	UFUNCTION(BlueprintCallable, Category = "Objective")
		UQuestObjective* GetObjective();

	UFUNCTION(BlueprintCallable, Category = "Objective")
		void SetObjective(UQuestObjective* inObjective);

	UFUNCTION(BlueprintCallable, Category = "Objective")
	FString GetObjectiveName();

	UFUNCTION(BlueprintCallable, Category = "Objective")
	FString GetObjectiveText();

};
