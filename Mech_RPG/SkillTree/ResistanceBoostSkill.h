// Copyright of Explosive Industries

#pragma once

#include "CoreMinimal.h"
#include "SkillTree/SkillTreeNode.h"
#include "ResistanceBoostSkill.generated.h"

/**
 * 
 */
UCLASS()
class MECH_RPG_API UResistanceBoostSkill : public USkillTreeNode
{
	GENERATED_BODY()
public:
		float GetStatBonus(EStatEnum statType) override;

	static UResistanceBoostSkill* CreateResistanceBoostSkill();

	UFUNCTION(BlueprintCallable, Category = "Skill Tree Node")
		FString GetTooltipText() override;
};