// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Ability.h"
#include "Mech_RPGCharacter.h"
#include "Stun.generated.h"

/**
 * 
 */
UCLASS()
class MECH_RPG_API UStun : public UAbility
{
	GENERATED_BODY()
private:
	float duration;

	UPROPERTY()
	class AMech_RPGCharacter* targetCharacter;

public :
	static UStun* CreateAbility(float cooldown, AMech_RPGCharacter* owner, float duration);
	void Activate(AMech_RPGCharacter* target, FVector targetLocation) override;
	
	void StunEnd();

	FTimerHandle TimerHandle_StunEnd;
	
};
