// Fill out your copyright notice in the Description page of Project Settings.

#include "Mech_RPG.h"
#include "ChannelledAbility.h"
#include "Mech_RPGCharacter.h"

void UChannelledAbility::Activate(AMech_RPGCharacter* target, FVector targetLocation) {
	if (!channelling) {
		channelling = true; 
		owner->SetChannelling(true);
		currentChannelTime = channelDuration - 0.1F;
		owner->GetCanMove()++;
		owner->GetCanAttack()++;
		targetCharacter = target;
		this->targetLocation = targetLocation;
		owner->GetWorld()->GetTimerManager().SetTimer(TimerHandle_AbilityOffCooldown, this, &UChannelledAbility::ActiveChannelAbility, 0.1F);
	}
}

bool UChannelledAbility::Channelling() {
	return channelling;
}

float UChannelledAbility::GetCooldown() {
	return abilityToActivate->GetCooldown();
}

float UChannelledAbility::GetChannelDuration() {
	return channelDuration;
}

float UChannelledAbility::GetCurrentChannelTime() {
	return currentChannelTime;
}

bool UChannelledAbility::OnCooldown() {
	return abilityToActivate->OnCooldown();
}

float UChannelledAbility::GetCurrentTimeRemaining() {
	return abilityToActivate->GetCurrentTimeRemaining();
}

void UChannelledAbility::ActiveChannelAbility() {
	if (channelling && owner->Channelling() && !owner->IsDead()) {
		if (currentChannelTime > 0) {
			currentChannelTime -= 0.1F;
			owner->GetWorld()->GetTimerManager().SetTimer(TimerHandle_AbilityOffCooldown, this, &UChannelledAbility::ActiveChannelAbility, 0.1F);
		}
		else {
			owner->SetChannelling(false);
			channelling = false;
			abilityToActivate->Activate(targetCharacter, targetLocation);
			owner->GetCanMove()--;
			owner->GetCanAttack()--;
		}
	}
	else {
		owner->SetChannelling(false);
		channelling = false;
		currentChannelTime = 0;
		owner->GetCanMove()--;
		owner->GetCanAttack()--;
		targetCharacter = NULL;
		targetLocation = FVector::ZeroVector;
	}
}

UChannelledAbility* UChannelledAbility::CreateChannelledAbility(AMech_RPGCharacter* inOwner, UAbility* inAbilityToActivate, float inChannelDuration, bool inUsesLocation) {
	UChannelledAbility* ability = NewObject<UChannelledAbility>(StaticClass());
	ability->owner = inOwner;
	ability->abilityToActivate = inAbilityToActivate;
	ability->channelDuration = inChannelDuration;
	ability->usesLocation = inUsesLocation;
	return ability;
}