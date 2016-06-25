// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Ability.h"
#include "Characters/Mech_RPGCharacter.h"
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
	bool Activate(class AMech_RPGCharacter* target, FVector targetLocation = FVector::ZeroVector) override;

	UFUNCTION(BlueprintCallable, Category = "Ability")
		virtual FString GetTooltipText() override;
	
	void StunEnd();

	FTimerHandle TimerHandle_StunEnd;
	
};
