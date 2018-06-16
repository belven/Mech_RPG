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
	 TMap<EStatEnum, float> GetStatBonuses() override;
	
	
};
