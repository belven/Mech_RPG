// Copyright of Explosive Industries

#pragma once

#include "Abilities/Ability.h"
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

	static const int radius;
public:

	UFUNCTION(BlueprintCallable, Category = "Ability")
		bool Activate(class AMech_RPGCharacter* target, FVector targetLocation = FVector::ZeroVector) override;

	UFUNCTION(BlueprintCallable, Category = "Ability")
		static UAoEHeal* CreateAbility(float cooldown, AMech_RPGCharacter* owner, float inHealAmount);

	UFUNCTION(BlueprintCallable, Category = "Ability")
		virtual FString GetTooltipText() override;	
};
