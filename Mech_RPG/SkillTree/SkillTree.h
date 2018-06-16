// Copyright of Explosive Industries

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Enums.h"
#include "SkillTree.generated.h"

/**
 *
 */
UCLASS()
class MECH_RPG_API USkillTree : public UObject
{
	GENERATED_BODY()
private:
	AMech_RPGCharacter * owner;
	ESpecialisation spec;

public:
	float GetStatBonus(EStatEnum statType);

	UFUNCTION(BlueprintCallable, Category = "Skill Tree")
		AMech_RPGCharacter * GetOwner() { return owner; }

	UFUNCTION(BlueprintCallable, Category = "Skill Tree")
		void SetOwner(AMech_RPGCharacter * val) { owner = val; }

	UFUNCTION(BlueprintCallable, Category = "Skill Tree")
		ESpecialisation GetSpec() { return spec; }

	UFUNCTION(BlueprintCallable, Category = "Skill Tree")
		void SetSpec(ESpecialisation val) { spec = val; }

	UFUNCTION(BlueprintCallable, Category = "Skill Tree")
		static USkillTree* CreateSkillTree(AMech_RPGCharacter * newOwner, ESpecialisation specialisation);
};
