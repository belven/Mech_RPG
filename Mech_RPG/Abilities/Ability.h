// Fill out your copyright notice in the Description page of Project Settings.

UENUM(BlueprintType)
namespace AbilityEnums {
	enum Ability {
		AoEHeal,
		CritBoost,
		Disable,
		DefenceBoost,
		Grenade,
		Heal,
		Snipe,
		Shield,
		Stun,
		Taunt,
		End
	};
}



#pragma once
#include "MiscLibrary.h"
#include "AOEHealthChange.h"
#include "Tag.h"
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
	TArray<FTag> tags;
	static TArray<TEnumAsByte<AbilityEnums::Ability>> supportAbilities;
	static TArray<TEnumAsByte<AbilityEnums::Ability>> offensiveAbilities;
	static TArray<TEnumAsByte<AbilityEnums::Ability>> defensiveAbilities;
public:
	static const FString healTag;
	static const FString damageTag;
	static const FString buffTag;
	static const FString debuffTag;
	static const FString aoeTag;
	static const FString needsTargetTag;

	virtual bool HasTag(FString name);
	virtual TArray<FTag>& GetTags();
	virtual FTag GetTag(FString name);

	virtual void AddTag(FString name, float value);
	virtual void RemoveTag(FString name);

	virtual float GetTagValue(FString name);
	virtual bool GetTagTrue(FString name);

	float GetWeaponHealthChange();

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
		virtual FString GetTooltipText();

	UFUNCTION(BlueprintCallable, Category = "Ability")
		virtual bool Activate(class AMech_RPGCharacter* target, FVector targetLocation = FVector::ZeroVector) { check(0 && "You must override this") return false; };

	FTimerHandle TimerHandle_AbilityOffCooldown;

	TEnumAsByte<AOEEnums::AffectedTeam> GetAffectedTeam();

	static UAbility* CreateChannelledPresetAbility(AMech_RPGCharacter* owner, AbilityEnums::Ability abilityToCreate, float inChannelDuration, bool inUsesLocation = false, bool inUsesTrace = false);

	static UAbility* CreatePresetAbility(AMech_RPGCharacter* owner, AbilityEnums::Ability abilityToCreate);

	static TArray<TEnumAsByte<AbilityEnums::Ability>> CreateSupportAbilityList();

	static TArray<TEnumAsByte<AbilityEnums::Ability>> CreateOffensiveAbilityList();

	static TArray<TEnumAsByte<AbilityEnums::Ability>> CreateDefensiveAbilityList();

};
