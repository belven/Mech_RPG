// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Ability.h"
#include "DamageBoost.generated.h"

/**
 * 
 */
UCLASS()
class MECH_RPG_API UDamageBoost : public UAbility
{
	GENERATED_BODY()

private:
	float damageMultiplier;
public:

	UFUNCTION(BlueprintCallable, Category = "Ability")
		void Activate(AMech_RPGCharacter* target, FVector targetLocation) override;

	UFUNCTION(BlueprintCallable, Category = "Ability")
		static UDamageBoost* CreateAbility(float cooldown, AMech_RPGCharacter* owner, float inDamageMultiplier);

	void ResetDamageBoost();

	FTimerHandle TimerHandle_DamageBoostEnded;
};
