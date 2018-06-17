// Copyright of Explosive Industries

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Enums.h"
#include "SkillTreeNode.generated.h"

/**
 *
 */
UCLASS(Blueprintable)
class MECH_RPG_API USkillTreeNode : public UObject
{
	GENERATED_BODY()

private:
	int id;
	FString name;
	int startLevel;
	int currentLevel;
	int maxLevel;

public:
	virtual float GetStatBonus(EStatEnum statType)
	{
		check(0 && "You must override this") return 0;
	};

	UFUNCTION(BlueprintCallable, Category = "Skill Tree Node")
		int GetId() { return id; }

	UFUNCTION(BlueprintCallable, Category = "Skill Tree Node")
		void SetId(int val) { id = val; }

	UFUNCTION(BlueprintCallable, Category = "Skill Tree Node")
		FString GetName() { return name; }

	UFUNCTION(BlueprintCallable, Category = "Skill Tree Node")
		void SetName(FString val) { name = val; }

	UFUNCTION(BlueprintCallable, Category = "Skill Tree Node")
		int GetStartLevel() { return startLevel; }

	UFUNCTION(BlueprintCallable, Category = "Skill Tree Node")
		void SetStartLevel(int val) { startLevel = val; }

	UFUNCTION(BlueprintCallable, Category = "Skill Tree Node")
		int GetCurrentLevel() { return currentLevel; }

	UFUNCTION(BlueprintCallable, Category = "Skill Tree Node")
		void SetCurrentLevel(int val) { currentLevel = val; }

	UFUNCTION(BlueprintCallable, Category = "Skill Tree Node")
		int GetMaxLevel() { return maxLevel; }

	UFUNCTION(BlueprintCallable, Category = "Skill Tree Node")
		void SetMaxLevel(int val) { maxLevel = val; }

	UFUNCTION(BlueprintCallable, Category = "Skill Tree Node")
		virtual FString GetTooltipText();
};
