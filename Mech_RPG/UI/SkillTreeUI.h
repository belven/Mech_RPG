// Copyright of Explosive Industries

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Enums.h"
#include "SkillTreeUI.generated.h"

class USkillTree;
class USkillTreeNode;

UCLASS()
class MECH_RPG_API USkillTreeUI : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY()
		USkillTree * skillTree;

public:
	UFUNCTION(BlueprintCallable, Category = "Skill Tree")
		int GetLevel();

	UFUNCTION(BlueprintCallable, Category = "Skill Tree")
		ESpecialisation GetSpec();
	
	UFUNCTION(BlueprintCallable, Category = "Skill Tree Nodes")
		TArray<USkillTreeNode *>& GetNodes();

	UFUNCTION(BlueprintCallable, Category = "Skill Tree")
		USkillTree * GetSkillTree() { return skillTree; }

	UFUNCTION(BlueprintCallable, Category = "Skill Tree")
		void SetSkillTree(USkillTree * val) { skillTree = val; }
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Skill Tree")
		void GenerateNodes();
};
