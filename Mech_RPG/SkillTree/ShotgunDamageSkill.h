// Copyright of Explosive Industries

#pragma once

#include "CoreMinimal.h"
#include "SkillTree/SkillTreeNode.h"
#include "ShotgunDamageSkill.generated.h"

/**
 * 
 */
UCLASS()
class MECH_RPG_API UShotgunDamageSkill : public USkillTreeNode
{
	GENERATED_BODY()
public:
	float GetStatBonus(EStatEnum statType) override;

	static UShotgunDamageSkill* CreateShotgunDamageSkill();

	UFUNCTION(BlueprintCallable, Category = "Skill Tree Node")
		FString GetTooltipText() override;

	UFUNCTION(BlueprintCallable, Category = "Skill Tree Node")
		void OwnerPreChangedOthersHealth(FHealthChange& healthChange) override;
	
};
