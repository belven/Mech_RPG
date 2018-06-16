// Copyright of Explosive Industries

#pragma once

#include "CoreMinimal.h"
#include "SkillTree/SkillTreeNode.h"
#include "HealthChangeSkill.generated.h"

/**
 * 
 */
UCLASS()
class MECH_RPG_API UHealthChangeSkill : public USkillTreeNode
{
	GENERATED_BODY()
public:
	TMap<EStatEnum, float> GetStatBonuses() override;
	
	
	
};
