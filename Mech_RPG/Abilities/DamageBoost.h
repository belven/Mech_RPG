// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Abilities/Ability.h"
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
		bool Activate(class AMech_RPGCharacter* target, FVector targetLocation = FVector::ZeroVector) override;

	UFUNCTION(BlueprintCallable, Category = "Ability")
		static UDamageBoost* CreateAbility(float cooldown, AMech_RPGCharacter* owner, float inDamageMultiplier);

	UFUNCTION(BlueprintCallable, Category = "Ability")
		virtual FString GetTooltipText() override;
};
