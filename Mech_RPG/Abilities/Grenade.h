// Copyright of Explosive Industries

#pragma once

#include "Abilities/Ability.h"
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
		bool Activate(class AMech_RPGCharacter* target, FVector targetLocation = FVector::ZeroVector) override;

	UFUNCTION(BlueprintCallable, Category = "Ability")
		static UGrenade* CreateAbility(float cooldown, AMech_RPGCharacter* owner, float inDamage);

	UFUNCTION(BlueprintCallable, Category = "Ability")
		virtual FString GetTooltipText() override;
	
};
