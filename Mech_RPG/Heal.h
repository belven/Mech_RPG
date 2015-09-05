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
	void Activate(AMech_RPGCharacter* target, FVector targetLocation) override;
	static UHeal* CreateAbility(float cooldown, AMech_RPGCharacter* owner, float inHealAmount);
	
};
