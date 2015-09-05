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
		void Activate(AMech_RPGCharacter* target, FVector targetLocation) override;
		static UDamageBoost* CreateAbility(float cooldown, AMech_RPGCharacter* owner, float inDamageMultiplier);

		void ResetDamageBoost();
};
