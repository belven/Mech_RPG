// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Ability.h"
#include "TargetedHealthChange.h"
#include "Snipe.generated.h"

/**
 *
 */
UCLASS()
class MECH_RPG_API USnipe : public UTargetedHealthChange
{
	GENERATED_BODY()
public:
	
	UFUNCTION(BlueprintCallable, Category = "Ability")
	static USnipe* CreateAbility(float cooldown,class AMech_RPGCharacter* owner);
};
