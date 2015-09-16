// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "Mech_RPG.h"
#include "Ability.h"
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