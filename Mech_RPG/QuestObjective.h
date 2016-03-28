// Copyright of Explosive Industries

#pragma once

#include "Object.h"
#include "QuestObjective.generated.h"

class UQuest;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FObjectiveCompleted, UQuestObjective*, objective);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FObjectiveUpdated, UQuestObjective*, objective);

UCLASS(Blueprintable)
class MECH_RPG_API UQuestObjective : public UObject
{
	GENERATED_BODY()
private:

	FString objectiveName;

	UQuest* quest;
	bool complete;
public:
	UPROPERTY(BlueprintAssignable, Category = "Events")
		FObjectiveCompleted OnObjectiveCompleted;

	UPROPERTY(BlueprintAssignable, Category = "Events")
		FObjectiveUpdated OnObjectiveUpdated;

	UFUNCTION(BlueprintCallable, Category = Quest)
		virtual void SetUpListeners(UQuest* quest);

	UFUNCTION(BlueprintCallable, Category = Quest)
		virtual bool IsComplete() { check(0 && "You must override this"); return false; };
	
	UFUNCTION(BlueprintCallable, Category = Quest)
		UQuest* GetQuest();

	UFUNCTION(BlueprintCallable, Category = Quest)
		void SetQuest(UQuest* newVal);

	UFUNCTION(BlueprintCallable, Category = QuestObjective)
		void SetQuestObjective(FString objectiveName = "Test Objective");

	UFUNCTION(BlueprintCallable, Category = Quest)
		void NotifyQuest();

	/** text describing the objective */
	UFUNCTION(BlueprintCallable, Category = Quest)
		virtual FString GetObjectiveText() { check(0 && "You must override this"); return "Not Implemented"; }

	UFUNCTION(BlueprintCallable, Category = Quest)
		FString GetObjectiveName();

	UFUNCTION(BlueprintCallable, Category = Quest)
		void SetObjectiveName(FString newName);
};
