// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "Mech_RPG.h"
#include "Ability.h"
#include "ChannelledAbility.h"
#include "Mech_RPGCharacter.h"

void UAbility::SetOnCooldown(UWorld* const World) {
	onCooldown = true;
	currentTime = GetCooldown() - 0.1;
	World->GetTimerManager().SetTimer(TimerHandle_AbilityOffCooldown, this, &UAbility::ResetOnCooldown, 0.1F);
}

float UAbility::GetCooldown() {
	return cooldown;
}

float UAbility::GetCurrentTimeRemaining() {
	return currentTime;
}

bool UAbility::OnCooldown() {
	return onCooldown;
}

void UAbility::SetCooldown(float newCooldown) {
	cooldown = newCooldown;
}

void UAbility::ResetOnCooldown() {
	if (GetCurrentTimeRemaining() <= 0) {
		onCooldown = false;
		currentTime = 0;
	}
	else {
		currentTime -= 0.1;
		owner->GetWorld()->GetTimerManager().SetTimer(TimerHandle_AbilityOffCooldown, this, &UAbility::ResetOnCooldown, 0.1F);
	}
}

TEnumAsByte<AOEEnums::AffectedTeam> UAbility::GetAffectedTeam() {
	return affectedTeam;
}

UAbility* UAbility::CreateChannelledPresetAbility(AMech_RPGCharacter * owner, AbilityEnums::Ability abilityToCreate, float inChannelDuration, bool inUsesLocation, bool inUsesTrace)
{
	return UChannelledAbility::CreateChannelledAbility(owner, CreatePresetAbility(owner, abilityToCreate), inChannelDuration, inUsesLocation, inUsesTrace);
}

UAbility* UAbility::CreatePresetAbility(AMech_RPGCharacter* owner, AbilityEnums::Ability abilityToCreate)
{
	switch (abilityToCreate) {
	case AbilityEnums::Heal:
		return UHeal::CreateAbility(15.0F, owner, 500.0F);
	case AbilityEnums::AoEHeal:
		return UAoEHeal::CreateAbility(20.0F, owner, 0.1F);
	case AbilityEnums::Stun:
		return UStun::CreateAbility(10.0F, owner, 4);
	case AbilityEnums::Disable:
		return UDisable::CreateDisable(10.0F, owner, 4.0F);
	case AbilityEnums::Taunt:
		return UTaunt::CreateAbility(5.0F, owner);
	case AbilityEnums::Grenade:
		return UGrenade::CreateAbility(7.0F, owner, 600.0F);
	case AbilityEnums::CritBoost:
		return UCritBoost::CreateCritBoost(6, owner, 55.0F);
	case AbilityEnums::Snipe:
		return USnipe::CreateAbility(15.0F, owner);
	case AbilityEnums::DefenceBoost:
		return UDefenceBoost::CreateAbility(7.0F, owner, 0.25F);
	}

	return nullptr;
}
