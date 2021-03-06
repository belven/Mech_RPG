// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Ability.h"
#include "Taunt.generated.h"

/**
 * 
 */
UCLASS()
class MECH_RPG_API UTaunt : public UAbility
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, Category = "Ability")
	bool Activate(class AMech_RPGCharacter* target, FVector targetLocation = FVector::ZeroVector) override;

	UFUNCTION(BlueprintCallable, Category = "Ability")
	static UTaunt* CreateAbility(float cooldown, AMech_RPGCharacter* owner);
	
};
