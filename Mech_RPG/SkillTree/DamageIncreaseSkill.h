// Copyright of Explosive Industries

#pragma once

#include "CoreMinimal.h"
#include "SkillTree/SkillTreeNode.h"
#include "DamageIncreaseSkill.generated.h"

/**
 * 
 */
UCLASS()
class MECH_RPG_API UDamageIncreaseSkill : public USkillTreeNode
{
	GENERATED_BODY()
public:
	float GetStatBonus(EStatEnum statType) override;
	
	static UDamageIncreaseSkill* CreateDamageIncreaseSkill();
	
	
};
