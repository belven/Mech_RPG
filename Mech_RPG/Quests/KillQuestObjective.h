// Copyright of Explosive Industries

#pragma once

#include "QuestObjective.h"
#include "KillQuestObjective.generated.h"

UCLASS()
class MECH_RPG_API UKillQuestObjective : public UQuestObjective
{
	GENERATED_BODY()

private:
	int killCount = 0;
	int kills = 0;
	TSubclassOf<AMech_RPGCharacter> killClass;

public:
	UFUNCTION(BlueprintCallable, Category = "Kill Quest Objective")
		virtual void SetUpListeners(UQuest* inQuest) override;

	UFUNCTION(BlueprintCallable, Category = "Kill Quest Objective")
		virtual bool IsComplete() override;

	UFUNCTION(BlueprintCallable, Category = "Kill Quest Objective")
		virtual FString GetObjectiveText();

	UFUNCTION(BlueprintCallable, Category = "Kill Quest Objective")
		void EnemyKilled(AMech_RPGCharacter* character);

	UFUNCTION(BlueprintCallable, Category = "Kill Quest Objective")
		static UKillQuestObjective* CreateKillQuestObjective(FString name, int32 inKillCount, TSubclassOf<AMech_RPGCharacter> inKillClass);
};
