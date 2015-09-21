// Copyright of Explosive Industries

#pragma once

#include "Ability.h"
#include "Grenade.generated.h"

/**
 * 
 */
UCLASS()
class MECH_RPG_API UGrenade : public UAbility
{
	GENERATED_BODY()
	
private:
	float damage;

public:

	UFUNCTION(BlueprintCallable, Category = "Ability")
		void Activate(AMech_RPGCharacter* target, FVector targetLocation) override;

	UFUNCTION(BlueprintCallable, Category = "Ability")
		static UGrenade* CreateAbility(float cooldown, AMech_RPGCharacter* owner, float inDamage);
	
};
