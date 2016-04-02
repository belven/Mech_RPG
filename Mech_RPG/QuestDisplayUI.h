// Copyright of Explosive Industries

#pragma once

#include "Blueprint/UserWidget.h"
#include "QuestDisplayUI.generated.h"

class AMech_RPGCharacter;
class Quest;

UCLASS()
class MECH_RPG_API UQuestDisplayUI : public UUserWidget
{
	GENERATED_BODY()

protected:
	AMech_RPGCharacter* character;

public:
	UFUNCTION(BlueprintCallable, Category = "Quests")
		AMech_RPGCharacter* GetCharacter();

	UFUNCTION(BlueprintCallable, Category = "Quests")
		void SetCharacter(AMech_RPGCharacter* inCharacter);

	UFUNCTION(BlueprintCallable, Category = "Quests")
		TArray<UQuest*>& GetQuests();

	UFUNCTION(BlueprintImplementableEvent, Category = "Quests")
		void GenerateQuests();

};
