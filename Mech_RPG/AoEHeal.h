// Copyright of Explosive Industries

#pragma once

#include "Ability.h"
#include "AoEHeal.generated.h"

/**
 * 
 */
UCLASS()
class MECH_RPG_API UAoEHeal : public UAbility
{
	GENERATED_BODY()

private:
	float healAmount;

public:

	UFUNCTION(BlueprintCallable, Category = "Ability")
		void Activate(AMech_RPGCharacter* target, FVector targetLocation) override;

	UFUNCTION(BlueprintCallable, Category = "Ability")
		static UAoEHeal* CreateAbility(float cooldown, AMech_RPGCharacter* owner, float inHealAmount);
	
	
	
	
};
