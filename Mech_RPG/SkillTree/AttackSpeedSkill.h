// Copyright of Explosive Industries

#pragma once

#include "CoreMinimal.h"
#include "SkillTree/SkillTreeNode.h"
#include "AttackSpeedSkill.generated.h"

/**
 * 
 */
UCLASS()
class MECH_RPG_API UAttackSpeedSkill : public USkillTreeNode
{
	GENERATED_BODY()
public:
	float GetStatBonus(EStatEnum statType) override;

	static UAttackSpeedSkill* CreateAttackSpeedSkill();

	UFUNCTION(BlueprintCallable, Category = "Skill Tree Node")
		FString GetTooltipText() override;	
};