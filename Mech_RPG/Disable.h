// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Ability.h"
#include "Mech_RPGCharacter.h"
#include "Disable.generated.h"

/**
 * 
 */
UCLASS()
class MECH_RPG_API UDisable : public UAbility
{
	GENERATED_BODY()
	
	
public:
	void Activate(AMech_RPGCharacter* target, FVector targetLocation) override;
	
};
