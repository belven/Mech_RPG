// Copyright of Explosive Industries

#pragma once

#include "Ability.h"
#include "TimedHealthChange.generated.h"

/**
 *
 */
UCLASS()
class MECH_RPG_API UTimedHealthChange : public UAbility {
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "Ability")
		bool Activate(class AMech_RPGCharacter* target, FVector targetLocation = FVector::ZeroVector) override;

	UFUNCTION(BlueprintCallable, Category = "Ability")
		void TimeTick();

	UFUNCTION(BlueprintCallable, Category = "Ability")
		static UTimedHealthChange* CreateTimedHealthChange(AMech_RPGCharacter* inOwner, float cooldown = 1.0F, float inChangeAmount = 1.0F, float inRate = 0.2F, float inDuration = 1.0F, bool inHeals = false);

private:
	float changeAmount = 100;
	float rate = 0.1F;
	float duration = 1.0F;
	float timeLeft = 1.0F;
	bool heals;

	class AMech_RPGCharacter* targetCharacter;

	FTimerHandle TimerHandle_TimeTick;
};
