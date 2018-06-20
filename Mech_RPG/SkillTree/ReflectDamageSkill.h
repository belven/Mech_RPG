// Copyright of Explosive Industries

#pragma once

#include "CoreMinimal.h"
#include "SkillTree/SkillTreeNode.h"
#include "ReflectDamageSkill.generated.h"

/**
 * 
 */
UCLASS()
class MECH_RPG_API UReflectDamageSkill : public USkillTreeNode
{
	GENERATED_BODY()
public:
		float GetStatBonus(EStatEnum statType) override;

	static UReflectDamageSkill* CreateReflectDamageSkill();

	UFUNCTION(BlueprintCallable, Category = "Skill Tree Node")
		FString GetTooltipText() override;

	UFUNCTION(BlueprintCallable, Category = "Skill Tree Node")
		void PreHealthChanged(FHealthChange& healthChange) override;
	
	
	
};
