// Fill out your copyright notice in the Description page of Project Settings.

#include "Mech_RPG.h"
#include "Heal.h"
#include "Mech_RPGCharacter.h"


void UHeal::Activate(AMech_RPGCharacter* target, FVector targetLocation) {
	if (target && target->GetMaxHealth() - target->GetHealth() < healAmount) {
		SetOnCooldown(owner->GetWorld());
		target->Hit(owner, -healAmount);
	}
}


UHeal* UHeal::CreateAbility(float cooldown, AMech_RPGCharacter* owner, float inHealAmount) {
	UHeal* ability = NewObject<UHeal>(StaticClass());
	ability->SetCooldown(cooldown);
	ability->healAmount = inHealAmount;
	ability->owner = owner;
	return ability;
}