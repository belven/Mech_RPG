// Copyright of Explosive Industries

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillTreeNodeUI.generated.h"

class USkillTreeNode;

UCLASS()
class MECH_RPG_API USkillTreeNodeUI : public UUserWidget
{
	GENERATED_BODY()
private:
	USkillTreeNode * owningNode;		
public:

	UFUNCTION(BlueprintCallable, Category = "Skill Tree Node")
		int GetId();
	
	UFUNCTION(BlueprintCallable, Category = "Skill Tree Node")
		FString GetName();
	
	UFUNCTION(BlueprintCallable, Category = "Skill Tree Node")
		int GetStartLevel();
	
	UFUNCTION(BlueprintCallable, Category = "Skill Tree Node")
		int GetCurrentLevel();

	UFUNCTION(BlueprintCallable, Category = "Skill Tree Node")
		void SetCurrentLevel(int val);

	UFUNCTION(BlueprintCallable, Category = "Skill Tree Node")
		int GetMaxLevel();

	UFUNCTION(BlueprintCallable, Category = "Skill Tree Node")
		USkillTreeNode * GetOwningNode() { return owningNode; }

	UFUNCTION(BlueprintCallable, Category = "Skill Tree Node")
		void SetOwningNode(USkillTreeNode * val) { owningNode = val; }
	
	UFUNCTION(BlueprintCallable, Category = "Skill Tree Node")
		FString GetTooltipText();

};
