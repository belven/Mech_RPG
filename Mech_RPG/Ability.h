// Fill out your copyright notice in the Description page of Project Settings.

UENUM(BlueprintType)
namespace AbilityEnums {
	enum Ability {
		Heal,
		AoEHeal,
		Stun,
		Disable,
		Taunt,
		Grenade,
		CritBoost,
		Snipe,
		DefenceBoost,
		End
	};
}

#pragma once
#include "AOEHealthChange.h"
#include "Ability.generated.h"

UCLASS(Blueprintable)
class UAbility : public UObject {
	GENERATED_BODY()

protected:
	class AMech_RPGCharacter* owner;

private:
	float cooldown = 1.0F;

	bool onCooldown = false;

	float currentTime = 0.0F;
protected:
	TEnumAsByte<AOEEnums::AffectedTeam> affectedTeam = AOEEnums::Enemy;

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

	TEnumAsByte<AOEEnums::AffectedTeam> GetAffectedTeam();

	static UAbility* CreateChannelledPresetAbility(AMech_RPGCharacter* owner, AbilityEnums::Ability abilityToCreate, float inChannelDuration, bool inUsesLocation = false, bool inUsesTrace = false);

	static UAbility* CreatePresetAbility(AMech_RPGCharacter* owner, AbilityEnums::Ability abilityToCreate);

};
