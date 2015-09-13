// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Ability.h"
#include "Mech_RPGCharacter.h"
#include "Immobilise.generated.h"

/**
 * 
 */
UCLASS()
class MECH_RPG_API UImmobilise : public UAbility
{
	GENERATED_BODY()
private: 
	float duration; 
	AMech_RPGCharacter* lastTarget;
public:

	void Activate(AMech_RPGCharacter* target, FVector targetLocation) override;
	

	UFUNCTION(BlueprintCallable, Category = "Ability")
		static UImmobilise* CreateAbility(float cooldown, AMech_RPGCharacter* owner, float duration);
	void ResetImmobilise(); 
	FTimerHandle TimerHandle_ImmobiliseEnded;
};
