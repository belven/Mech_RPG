// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Ability.h"
#include "TargetedHealthChange.h"
#include "Heal.generated.h"

/**
 * 
 */
UCLASS()
class MECH_RPG_API UHeal : public UTargetedHealthChange
{
	GENERATED_BODY()
private:
	float healAmount;

public:

	UFUNCTION(BlueprintCallable, Category = "Ability")
	static UHeal* CreateAbility(float cooldown, AMech_RPGCharacter* owner, float inHealAmount);
	
};
