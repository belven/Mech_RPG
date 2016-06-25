// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Abilities/Ability.h"
#include "Characters/Mech_RPGCharacter.h"
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

	bool Activate(class AMech_RPGCharacter* target, FVector targetLocation = FVector::ZeroVector) override;

	UFUNCTION(BlueprintCallable, Category = "Ability")
		static UImmobilise* CreateAbility(float cooldown, AMech_RPGCharacter* owner, float duration);

	UFUNCTION(BlueprintCallable, Category = "Ability")
		virtual FString GetTooltipText() override;

	void ResetImmobilise();
	FTimerHandle TimerHandle_ImmobiliseEnded;
};
