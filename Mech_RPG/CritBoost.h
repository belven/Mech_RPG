// Copyright of Explosive Industries

#pragma once

#include "Ability.h"
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
	class AWeapon* weaponAffected;
public:

	UFUNCTION(BlueprintCallable, Category = "Ability")
		void Activate(AMech_RPGCharacter* target, FVector targetLocation) override;

	UFUNCTION(BlueprintCallable, Category = "Ability")
		static UCritBoost* CreateCritBoost(float cooldown, AMech_RPGCharacter* owner, float inCritMultiplier);

	void ResetCritBoost();

	FTimerHandle TimerHandle_DamageBoostEnded;	
};
