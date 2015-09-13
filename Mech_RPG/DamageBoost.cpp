// Fill out your copyright notice in the Description page of Project Settings.

#include "Mech_RPG.h"
#include "DamageBoost.h"
#include "Mech_RPGCharacter.h"

void UDamageBoost::Activate(AMech_RPGCharacter* target, FVector targetLocation) {
	SetOnCooldown(owner->GetWorld());
	owner->GetWorld()->GetTimerManager().SetTimer(TimerHandle_DamageBoostEnded, this, &UDamageBoost::ResetDamageBoost, GetCooldown() * 0.5);
	owner->SetDamageModifier(owner->GetDamageModifier() + damageMultiplier);
}

UDamageBoost* UDamageBoost::CreateAbility(float cooldown, AMech_RPGCharacter* owner, float inDamageMultiplier) {
	UDamageBoost* ability = NewObject<UDamageBoost>(StaticClass());
	ability->SetCooldown(cooldown);
	ability->damageMultiplier = inDamageMultiplier;
	ability->owner = owner;
	return ability;
}

void  UDamageBoost::ResetDamageBoost() {
	owner->SetDamageModifier(owner->GetDamageModifier() - damageMultiplier);
}