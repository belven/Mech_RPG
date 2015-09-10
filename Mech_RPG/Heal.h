// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Ability.h"
#include "Heal.generated.h"

/**
 * 
 */
UCLASS()
class MECH_RPG_API UHeal : public UAbility
{
	GENERATED_BODY()
private:
	float healAmount;

public:

	UFUNCTION(BlueprintCallable, Category = "Ability")
	void Activate(AMech_RPGCharacter* target, FVector targetLocation) override;

	UFUNCTION(BlueprintCallable, Category = "Ability")
	static UHeal* CreateAbility(float cooldown, AMech_RPGCharacter* owner, float inHealAmount);
	
};
