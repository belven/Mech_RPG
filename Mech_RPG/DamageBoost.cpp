// Fill out your copyright notice in the Description page of Project Settings.

#include "Mech_RPG.h"
#include "DamageBoost.h"
#include "Mech_RPGCharacter.h"

bool UDamageBoost::Activate(class AMech_RPGCharacter* target, FVector targetLocation) {
	SetOnCooldown(owner->GetWorld());
	owner->GetWorld()->GetTimerManager().SetTimer(TimerHandle_DamageBoostEnded, this, &UDamageBoost::ResetDamageBoost, GetCooldown() * 0.5);
	owner->SetHealthChangeModifier(owner->GetHealthChangeModifier() + damageMultiplier);
	return true;
}

UDamageBoost* UDamageBoost::CreateAbility(float cooldown, AMech_RPGCharacter* owner, float inDamageMultiplier) {
	UDamageBoost* ability = NewObject<UDamageBoost>(StaticClass());
	ability->SetCooldown(cooldown);
	ability->damageMultiplier = inDamageMultiplier;
	ability->owner = owner;
	ability->AddTag(buffTag, inDamageMultiplier);
	ability->AddTag(needsTargetTag, 0);
	return ability;
}

void  UDamageBoost::ResetDamageBoost() {
	owner->SetHealthChangeModifier(owner->GetHealthChangeModifier() - damageMultiplier);
}