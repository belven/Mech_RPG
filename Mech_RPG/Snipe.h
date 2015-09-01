// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Ability.h"
#include "Snipe.generated.h"

/**
 *
 */
UCLASS()
class MECH_RPG_API USnipe : public UAbility
{
	GENERATED_BODY()
public:
	void Activate(class AMech_RPGCharacter* owner, AMech_RPGCharacter* target, FVector targetLocation) override;

	static USnipe* CreateAbility(float cooldown);
};
