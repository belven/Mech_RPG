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

	UFUNCTION(BlueprintCallable, Category = "Ability")
	void Activate(AMech_RPGCharacter* target, FVector targetLocation) override;

	UFUNCTION(BlueprintCallable, Category = "Ability")
	static USnipe* CreateAbility(float cooldown,class AMech_RPGCharacter* owner);
};
