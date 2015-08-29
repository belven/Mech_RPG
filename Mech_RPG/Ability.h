// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "Ability.generated.h"

/**
 *
 */
UCLASS()
class  UAbility : public UObject
{
	GENERATED_BODY()

private:
	float cooldown;
	bool onCooldown;

public:
	void SetOnCooldown(UWorld* const World);
	bool GetOnCooldown();
	void ResetOnCooldown();

	float GetCooldown();
	void SetCooldown(float newCooldown);

	virtual void Activate(class AMech_RPGCharacter* owner) { check(0 && "You must override this") };

	static UAbility* CreateAbility();

	/** Handle for efficient management of ShotTimerExpired timer */
	FTimerHandle TimerHandle_ShotTimerExpired;
};
