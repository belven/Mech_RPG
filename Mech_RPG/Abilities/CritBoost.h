// Copyright of Explosive Industries

#pragma once

#include "Abilities/Ability.h"
#include "CritBoost.generated.h"

/**
 * 
 */
UCLASS()
class MECH_RPG_API UCritBoost : public UAbility
{
	GENERATED_BODY()
		
private:
	float critMultiplier;
	class UWeapon* weaponAffected;
	int count;
public:

	UFUNCTION(BlueprintCallable, Category = "Ability")
		bool Activate(class AMech_RPGCharacter* target, FVector targetLocation = FVector::ZeroVector) override;

	UFUNCTION(BlueprintCallable, Category = "Ability")
		static UCritBoost* CreateCritBoost(float cooldown, AMech_RPGCharacter* owner, float inCritMultiplier);

	UFUNCTION(BlueprintCallable, Category = "Ability")
		void OwnerFired();

	UFUNCTION(BlueprintCallable, Category = "Ability")
		virtual FString GetTooltipText() override;

	void ResetCritBoost();

	FTimerHandle TimerHandle_DamageBoostEnded;	
};
