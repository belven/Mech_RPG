// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Abilities/Ability.h"
#include "OrbitalStrike.generated.h"

/**
 * 
 */
UCLASS()
class MECH_RPG_API UOrbitalStrike : public UAbility
{
	GENERATED_BODY()
		float damage;
public:
	UOrbitalStrike();

	UFUNCTION(BlueprintCallable, Category = "Ability")
		bool Activate(class AMech_RPGCharacter* target, FVector targetLocation = FVector::ZeroVector) override;

	UFUNCTION(BlueprintCallable, Category = "Ability")
		static UOrbitalStrike* CreateAbility(float cooldown, AMech_RPGCharacter* owner, float inDamage);
	
	
	
};
