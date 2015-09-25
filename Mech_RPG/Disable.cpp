// Fill out your copyright notice in the Description page of Project Settings.

#include "Mech_RPG.h"
#include "Disable.h"


void UDisable::Activate(AMech_RPGCharacter* target, FVector targetLocation) {
	if (target != NULL && !target->IsDead()) {
		targetCharacter = target;
		targetCharacter->ApplyCrowdControl(EffectEnums::Cast, false);
		targetCharacter->ApplyCrowdControl(EffectEnums::Attack, false);
		owner->GetWorld()->GetTimerManager().SetTimer(TimerHandle_DisableEnd, this, &UDisable::DisableEnd, duration);
		SetOnCooldown(owner->GetWorld());
	}
}

UDisable* UDisable::CreateDisable(float cooldown, AMech_RPGCharacter* inOwner, float inDuration) {
	UDisable* ability = NewObject<UDisable>(StaticClass());
	ability->SetCooldown(cooldown);
	ability->owner = inOwner;
	ability->duration = inDuration;
	return ability;
}

void UDisable::DisableEnd() {
	targetCharacter->ApplyCrowdControl(EffectEnums::Cast, true);
	targetCharacter->ApplyCrowdControl(EffectEnums::Attack, true);
}