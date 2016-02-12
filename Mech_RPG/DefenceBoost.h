// Copyright of Explosive Industries

#pragma once

#include "Ability.h"
#include "DefenceBoost.generated.h"

/**
 * 
 */
UCLASS()
class MECH_RPG_API UDefenceBoost : public UAbility
{
	GENERATED_BODY()

		float DefenceModifier;
public:
		UFUNCTION(BlueprintCallable, Category = "Ability")
		bool Activate(class AMech_RPGCharacter* target, FVector targetLocation = FVector::ZeroVector) override;

	UFUNCTION(BlueprintCallable, Category = "Ability")
		static UDefenceBoost* CreateAbility(float cooldown, AMech_RPGCharacter* owner, float inDefenceMultiplier);

	void ResetDefenceBoost();

	FTimerHandle TimerHandle_DefenceBoostEnded;
	
	
};
