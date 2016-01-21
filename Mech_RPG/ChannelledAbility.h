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
	bool channelling = false;
	bool usesTrace = false;

	float channelDuration;
	float currentChannelTime = 0;

	UPROPERTY()
		UAbility* abilityToActivate;

	FCollisionQueryParams collision;
	FCollisionObjectQueryParams objectCollision;
	FHitResult hit;
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Particles)
		UParticleSystemComponent* partclSystem;

	UChannelledAbility();

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

	bool PerformLineTrace();

	UFUNCTION(BlueprintCallable, Category = "Ability")
		bool OnCooldown() override;

	UFUNCTION(BlueprintCallable, Category = "Ability")
		float GetCurrentTimeRemaining() override;

	UFUNCTION(BlueprintCallable, Category = "Ability")
		/*
Activates an ability at the end of a set duration, providing that:

1. The owner is still alive
2. The owner can cast and channelling is TRUE
3. If inUsesTrace is true and the line trace dectects a vaible target

Note: inUsesLocation will cause this to fixate on the original location that was passed in, i.e. the cursor location.
*/
static UChannelledAbility* CreateChannelledAbility(AMech_RPGCharacter* inOwner, UAbility* inAbilityToActivate, float inChannelDuration, bool inUsesLocation = false, bool inUsesTrace = false);
};
