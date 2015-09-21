// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Ability.h"
#include "ChannelledAbility.generated.h"

/**
 *
 */
UCLASS(Blueprintable)
class MECH_RPG_API UChannelledAbility : public UAbility {
	GENERATED_BODY()

private:

	UPROPERTY()
	class AMech_RPGCharacter* targetCharacter;

	FVector targetLocation;
	bool usesLocation = false;

	float channelDuration;

	UPROPERTY()
		UAbility* abilityToActivate;

	bool channelling = false;

	float currentChannelTime = 0;
public:

	UFUNCTION(BlueprintCallable, Category = "Ability")
		void Activate(AMech_RPGCharacter* target, FVector targetLocation) override;

	UFUNCTION(BlueprintCallable, Category = "Ability")
		float GetCooldown() override;

	UFUNCTION(BlueprintCallable, Category = "Ability")
		float GetCurrentChannelTime();

	UFUNCTION(BlueprintCallable, Category = "Ability")
		float GetChannelDuration();

	UFUNCTION(BlueprintCallable, Category = "Ability")
		bool Channelling();

	void ActiveChannelAbility();

	UFUNCTION(BlueprintCallable, Category = "Ability")
		bool OnCooldown() override;

	UFUNCTION(BlueprintCallable, Category = "Ability")
		float GetCurrentTimeRemaining() override;

	UFUNCTION(BlueprintCallable, Category = "Ability")
		static UChannelledAbility* CreateChannelledAbility(AMech_RPGCharacter* inOwner, UAbility* inAbilityToActivate, float inChannelDuration, bool inUsesLocation = false);
};
