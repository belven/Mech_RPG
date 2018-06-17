// Copyright of Explosive Industries

#pragma once

#include "CoreMinimal.h"
#include "SkillTreeNode.h"
#include "MaxHealthSkill.generated.h"

/**
 * 
 */
UCLASS()
class MECH_RPG_API UMaxHealthSkill : public USkillTreeNode
{
	GENERATED_BODY()
public:
	float GetStatBonus(EStatEnum statType) override;

	static UMaxHealthSkill* CreateMaxHealthSkill();

	UFUNCTION(BlueprintCallable, Category = "Skill Tree Node")
		FString GetTooltipText() override;	
};