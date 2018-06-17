// Copyright of Explosive Industries

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Enums.h"
#include "SkillTree.generated.h"

class USkillTreeNode;
class AMech_RPGCharacter;

UCLASS(Blueprintable)
class MECH_RPG_API USkillTree : public UObject
{
	GENERATED_BODY()
private:
	UPROPERTY()
		AMech_RPGCharacter * owner;

	UPROPERTY()
		ESpecialisation spec;

	UPROPERTY()
		TArray<USkillTreeNode*> nodes;

	UPROPERTY()
		float experience;

	void GainExpereince(float gainedExperience);

	float CheckForInvigoratorExperience(FHealthChange &healthChange);

	float CheckForDefilerExperience(FHealthChange &healthChange);

	float CheckForHavocExperience(FHealthChange &healthChange);

	float CheckForPacifierExperience(FHealthChange &healthChange);
public:
	UFUNCTION(BlueprintCallable, Category = "Skill Tree")
		int GetLevel();

	UFUNCTION(BlueprintCallable, Category = "Skill Tree")
		float GetStatBonus(EStatEnum statType);

	void OwnerChangedOthersHealth(FHealthChange& healthChange);

	UFUNCTION(BlueprintCallable, Category = "Skill Tree")
		AMech_RPGCharacter * GetTreeOwner() { return owner; }

	UFUNCTION(BlueprintCallable, Category = "Skill Tree")
		void SetTreeOwner(AMech_RPGCharacter * val) { owner = val; }

	UFUNCTION(BlueprintCallable, Category = "Skill Tree")
		ESpecialisation GetSpec() { return spec; }

	UFUNCTION(BlueprintCallable, Category = "Skill Tree")
		void SetSpec(ESpecialisation val) { spec = val; }

	UFUNCTION(BlueprintCallable, Category = "Skill Tree")
		static USkillTree* CreateSkillTree(AMech_RPGCharacter * newOwner, ESpecialisation specialisation);

	UFUNCTION(BlueprintCallable, Category = "Skill Tree Nodes")
		TArray<USkillTreeNode *>& GetNodes() { return nodes; }

	UFUNCTION(BlueprintCallable, Category = "Skill Tree Nodes")
		void SetNodes(TArray<USkillTreeNode *> val) { nodes = val; }

	UFUNCTION(BlueprintCallable, Category = "Skill Tree Nodes")
		void AddNode(USkillTreeNode* newNode);

	UFUNCTION(BlueprintCallable, Category = "Skill Tree Nodes")
		void RemoveNode(USkillTreeNode* nodeToRemove);
};
