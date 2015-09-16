// Fill out your copyright notice in the Description page of Project Settings.

#include "Mech_RPG.h"
#include "Heal.h"
#include "Mech_RPGCharacter.h"


void UHeal::Activate(AMech_RPGCharacter* target, FVector targetLocation) {
	if (target && target->GetMaxHealth() - target->GetHealth() < healAmount) {
		FDamage damage;

		damage.damager = owner;
		damage.target = target;
		damage.weaponUsed = NULL;
		damage.damagedDealt = healAmount;

		target->Hit(damage);
		SetOnCooldown(owner->GetWorld());
	}
}


UHeal* UHeal::CreateAbility(float cooldown, AMech_RPGCharacter* owner, float inHealAmount) {
	UHeal* ability = NewObject<UHeal>(StaticClass());
	ability->SetCooldown(cooldown);
	ability->healAmount = inHealAmount;
	ability->owner = owner;
	return ability;
}