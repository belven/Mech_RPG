// Fill out your copyright notice in the Description page of Project Settings.

#include "Mech_RPG.h"
#include "Abilities/DamageBoost.h"
#include "Characters/Mech_RPGCharacter.h"
#include "Delayed Events/ModifierTimer.h"

bool UDamageBoost::Activate(class AMech_RPGCharacter* target, FVector targetLocation) {
	SetOnCooldown(owner->GetWorld());
	TMap<ModifierEnums::ModifierType, float> modifiers;
	modifiers.Add(ModifierEnums::HealthChange, damageMultiplier);
	UModifierTimer::CreateEffectTimer(owner, GetCooldown() * 0.5, modifiers);
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

FString UDamageBoost::GetTooltipText()
{
	FString damageString = FString::SanitizeFloat(damageMultiplier < 2 ? damageMultiplier * 100 : damageMultiplier);
	return "Damage Boost" + UMiscLibrary::lnBreak + "Increases damage by " + damageString + UMiscLibrary::lnBreak + "Cooldown: " + FString::SanitizeFloat(GetCooldown());
}