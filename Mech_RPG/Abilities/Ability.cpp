// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "Mech_RPG.h"
#include "Abilities/Ability.h"
#include "Abilities.h"
#include "Characters/Mech_RPGCharacter.h"

const FString UAbility::healTag = "Heal";
const FString UAbility::damageTag = "Damage";
const FString UAbility::buffTag = "Buff";
const FString UAbility::debuffTag = "Debuff";
const FString UAbility::aoeTag = "AoE";
const FString UAbility::needsTargetTag = "Needs Target";
const FString UAbility::selfTargetted = "Self Target";


TArray<TEnumAsByte<AbilityEnums::Ability>> UAbility::supportAbilities = CreateSupportAbilityList();
TArray<TEnumAsByte<AbilityEnums::Ability>> UAbility::offensiveAbilities = CreateOffensiveAbilityList();
TArray<TEnumAsByte<AbilityEnums::Ability>> UAbility::defensiveAbilities = CreateDefensiveAbilityList();

TArray<TEnumAsByte<AbilityEnums::Ability>> UAbility::CreateSupportAbilityList()
{
	TArray<TEnumAsByte<AbilityEnums::Ability>> abilities;
	abilities.Add(AbilityEnums::Heal);
	abilities.Add(AbilityEnums::AoEHeal);
	return abilities;
}

TArray<TEnumAsByte<AbilityEnums::Ability>> UAbility::CreateOffensiveAbilityList()
{
	TArray<TEnumAsByte<AbilityEnums::Ability>> abilities;
	abilities.Add(AbilityEnums::Grenade);
	abilities.Add(AbilityEnums::Snipe);
	abilities.Add(AbilityEnums::CritBoost);
	return abilities;
}

TArray<TEnumAsByte<AbilityEnums::Ability>> UAbility::CreateDefensiveAbilityList()
{
	TArray<TEnumAsByte<AbilityEnums::Ability>> abilities;
	abilities.Add(AbilityEnums::Disable);
	abilities.Add(AbilityEnums::Stun);
	abilities.Add(AbilityEnums::Taunt);
	return abilities;
}

bool UAbility::HasTag(FString name)
{
	for (FTag tag : GetTags())
	{
		if (tag.name.Equals(name)) return true;
	}

	return false;
}

TArray<FTag>& UAbility::GetTags()
{
	return tags;
}

FTag UAbility::GetTag(FString name)
{
	for (FTag tag : GetTags())
	{
		if (tag.name.Equals(name)) return tag;
	}

	return FTag();
}

void UAbility::AddTag(FString name, float value)
{
	tags.Add(FTag(name, value));
}

void UAbility::RemoveTag(FString name)
{
	tags.Add(GetTag(name));
}

float UAbility::GetTagValue(FString name)
{
	return GetTag(name).value;
}

bool UAbility::GetTagTrue(FString name)
{
	if (HasTag(name))
	{
		return GetTag(name).value >= 1;
	}
	return false;
}

float UAbility::GetWeaponHealthChange()
{
	return UMiscLibrary::IsCharacterAlive(owner) && owner->GetCurrentWeapon() != nullptr ? owner->GetCurrentWeapon()->GetDPS() : 100;
}

void UAbility::SetOnCooldown(UWorld* const World)
{
	onCooldown = true;
	currentTime = GetCooldown() - 0.1;
	World->GetTimerManager().SetTimer(TimerHandle_AbilityOffCooldown, this, &UAbility::ResetOnCooldown, 0.1F);
}

float UAbility::GetCooldown()
{
	float percentChange = 1 - (owner->GetSpeedModifier() - 1);
	return cooldown  * percentChange;
}

float UAbility::GetCurrentTimeRemaining()
{
	return currentTime;
}

bool UAbility::OnCooldown()
{
	return onCooldown;
}

void UAbility::SetCooldown(float newCooldown)
{
	cooldown = newCooldown;
}

FString UAbility::GetTooltipText()
{
	return "";
}

void UAbility::ResetOnCooldown()
{
	if (GetCurrentTimeRemaining() <= 0)
	{
		onCooldown = false;
		currentTime = 0;
	}
	else
	{
		currentTime -= 0.1;
		owner->GetWorld()->GetTimerManager().SetTimer(TimerHandle_AbilityOffCooldown, this, &UAbility::ResetOnCooldown, 0.1F);
	}
}

EAffectedTeam UAbility::GetAffectedTeam()
{
	return affectedTeam;
}

UAbility* UAbility::CreateChannelledPresetAbility(AMech_RPGCharacter * owner, AbilityEnums::Ability abilityToCreate, float inChannelDuration, bool inUsesLocation, bool inUsesTrace)
{
	return UChannelledAbility::CreateChannelledAbility(owner, CreatePresetAbility(owner, abilityToCreate), inChannelDuration, inUsesLocation, inUsesTrace);
}

UAbility* UAbility::CreatePresetAbility(AMech_RPGCharacter* owner, AbilityEnums::Ability abilityToCreate)
{
	switch (abilityToCreate)
	{
	case AbilityEnums::Heal:
		return UHeal::CreateAbility(15, owner, 15);
	case AbilityEnums::AoEHeal:
		return UAoEHeal::CreateAbility(20, owner, 1);
	case AbilityEnums::Stun:
		return UStun::CreateAbility(15, owner, 4);
	case AbilityEnums::Disable:
		return UDisable::CreateDisable(10, owner, 4);
	case AbilityEnums::Taunt:
		return UTaunt::CreateAbility(5, owner);
	case AbilityEnums::Grenade:
		return UGrenade::CreateAbility(15, owner, 3);
	case AbilityEnums::CritBoost:
		return UCritBoost::CreateCritBoost(6, owner, 55);
	case AbilityEnums::Snipe:
		return USnipe::CreateAbility(15, owner);
	case AbilityEnums::DefenceBoost:
		return UDefenceBoost::CreateAbility(7, owner, 0.25F);
	case AbilityEnums::Shield:
		return UShield::CreateShield(20, owner, 40);
	case AbilityEnums::Immobilise:
		return UImmobilise::CreateAbility(15, owner, 5);
	case AbilityEnums::SummonDrone:
		return USummonDamageDrone::CreateAbility(15, owner);
	default:
		break;
	}

	return nullptr;
}
