// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "Ability.generated.h"

/**
 *
 */
UCLASS(Blueprintable)
class UAbility : public UObject {
	GENERATED_BODY()

protected:
	class AMech_RPGCharacter* owner;

private:
		float cooldown = 1.0F;

		bool onCooldown = false;

		float currentTime = 0.0F;

public:
	void SetOnCooldown(UWorld* const World);

	UFUNCTION(BlueprintCallable, Category = "Ability")
		virtual bool OnCooldown();
	void ResetOnCooldown();

	UFUNCTION(BlueprintCallable, Category = "Ability")
		virtual float GetCooldown();

	UFUNCTION(BlueprintCallable, Category = "Ability")
		virtual float GetCurrentTimeRemaining();

	UFUNCTION(BlueprintCallable, Category = "Ability")
		void SetCooldown(float newCooldown);

	UFUNCTION(BlueprintCallable, Category = "Ability")
		virtual void Activate(class AMech_RPGCharacter* target, FVector targetLocation = FVector::ZeroVector) { check(0 && "You must override this") };

	FTimerHandle TimerHandle_AbilityOffCooldown;
};
