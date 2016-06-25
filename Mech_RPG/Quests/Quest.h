// Copyright of Explosive Industries

#pragma once

#include "Object.h"
#include "Characters/Mech_RPGCharacter.h"
#include "Quest.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FQuestCompleted, UQuest*, quest);

class UQuestReward;
class UQuestObjective;

UCLASS()
class MECH_RPG_API UQuest : public UObject
{
	GENERATED_BODY()
private:

	/** quest name */
	UPROPERTY()
		FString questName;

	UPROPERTY()
		TArray<UQuestReward*> rewards;

	UPROPERTY()
		TArray<UQuestObjective*> objectives;

	UPROPERTY()
		AMech_RPGCharacter* owner;
public:

	UPROPERTY(BlueprintAssignable, Category = "Events")
		FQuestCompleted OnQuestCompleted;

	UFUNCTION(BlueprintCallable, Category = Quest)
		TArray<UQuestReward*>& GetRewards();

	UFUNCTION(BlueprintCallable, Category = Quest)
		void SetRewards(TArray<UQuestReward*> newVal);

	UFUNCTION(BlueprintCallable, Category = Quest)
		AMech_RPGCharacter* GetOwner();

	UFUNCTION(BlueprintCallable, Category = Quest)
		void SetOwner(AMech_RPGCharacter* newVal);

	UFUNCTION(BlueprintCallable, Category = Quest)
		TArray<UQuestObjective*>& GetObjectives();

	UFUNCTION(BlueprintCallable, Category = Quest)
		void SetObjectives(TArray<UQuestObjective*> newVal);

	UFUNCTION(BlueprintCallable, Category = Quest)
		static UQuest* CreateQuest(AMech_RPGCharacter* character, TArray<UQuestObjective*> inObjectives, TArray<UQuestReward*> inRewards, FString qName = "Default Quest");

	UFUNCTION(BlueprintCallable, Category = Quest)
		void ObjectiveUpdated(UQuestObjective* objective);

	UFUNCTION(BlueprintCallable, Category = Quest)
		bool IsComplete();

	UFUNCTION(BlueprintCallable, Category = Quest)
		void AddObjective(UQuestObjective* inObjective);

	UFUNCTION(BlueprintCallable, Category = Quest)
		void AddReward(UQuestReward* inReward);

	UFUNCTION(BlueprintCallable, Category = Quest)
		void GiveRewards();

	UFUNCTION(BlueprintCallable, Category = Quest)
		FString GetQuestName();

	UFUNCTION(BlueprintCallable, Category = Quest)
		void SetQuestName(FString newName);

	UFUNCTION(BlueprintCallable, Category = Quest)
		UQuestObjective* GetFirstObjective();

	UFUNCTION(BlueprintCallable, Category = Quest)
		void AbandonQuest();
};
