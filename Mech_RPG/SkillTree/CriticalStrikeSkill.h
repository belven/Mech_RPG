// Copyright of Explosive Industries

#pragma once

#include "CoreMinimal.h"
#include "SkillTree/SkillTreeNode.h"
#include "CriticalStrikeSkill.generated.h"

/**
 *
 */
UCLASS()
class MECH_RPG_API UCriticalStrikeSkill : public USkillTreeNode
{
	GENERATED_BODY()

public:
	float GetStatBonus(EStatEnum statType) override;
	static UCriticalStrikeSkill* CreateCriticalStrikeSkill();

	UFUNCTION(BlueprintCallable, Category = "Skill Tree Node")
		FString GetTooltipText() override;
};
