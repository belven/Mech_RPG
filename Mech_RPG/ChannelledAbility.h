// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Ability.h"
#include "ChannelledAbility.generated.h"

/**
 * 
 */
UCLASS()
class MECH_RPG_API UChannelledAbility : public UAbility
{
	GENERATED_BODY()
	
private:

	class AMech_RPGCharacter* targetCharacter;
	FVector targetLocation;
	bool usesLocation = false;

	float channelDuration;
	UAbility* abilityToActivate;
	bool channelling = false;
public:

	UFUNCTION(BlueprintCallable, Category = "Ability")
	void Activate(AMech_RPGCharacter* target, FVector targetLocation) override;

	void ActiveChannelAbility();
	bool OnCooldown() override;

	UFUNCTION(BlueprintCallable, Category = "Ability")
		static UChannelledAbility* CreateChannelledAbility(AMech_RPGCharacter* inOwner, UAbility* inAbilityToActivate, float inChannelDuration, bool inUsesLocation = false);
};
