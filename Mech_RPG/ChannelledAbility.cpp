// Fill out your copyright notice in the Description page of Project Settings.

#include "Mech_RPG.h"
#include "ChannelledAbility.h"
#include "Mech_RPGCharacter.h"

void UChannelledAbility::Activate(AMech_RPGCharacter* target, FVector targetLocation) {
	if (!channelling) {
		channelling = true;
		owner->GetWorld()->GetTimerManager().SetTimer(TimerHandle_AbilityOffCooldown, this, &UChannelledAbility::ActiveChannelAbility, channelDuration);
		owner->GetCanMove()++;
		owner->GetCanAttack()++;
		targetCharacter = target;
		this->targetLocation = targetLocation;
	}
}

float UChannelledAbility::GetCooldown() {
	return abilityToActivate->GetCooldown();
}

bool UChannelledAbility::OnCooldown() {
	return abilityToActivate->OnCooldown();
}

float UChannelledAbility::GetCurrentTimeRemaining() {
	return abilityToActivate->GetCurrentTimeRemaining();
}

void UChannelledAbility::ActiveChannelAbility() {
	channelling = false;
	abilityToActivate->Activate(targetCharacter, targetLocation);
	owner->GetCanMove()--;
	owner->GetCanAttack()--;
}

UChannelledAbility* UChannelledAbility::CreateChannelledAbility(AMech_RPGCharacter* inOwner, UAbility* inAbilityToActivate, float inChannelDuration, bool inUsesLocation) {
	UChannelledAbility* ability = NewObject<UChannelledAbility>(StaticClass());
	ability->owner = inOwner;
	ability->abilityToActivate = inAbilityToActivate;
	ability->channelDuration = inChannelDuration;
	ability->usesLocation = inUsesLocation;
	return ability;
}