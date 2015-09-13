#pragma once
#include "Mech_RPG.h"
#include "Snipe.h"
#include "Mech_RPGCharacter.h"

void USnipe::Activate(AMech_RPGCharacter* target, FVector targetLocation) {
	if (target) {
		FDamage damage;

		damage.damager = owner;
		damage.target = target;
		damage.weaponUsed = NULL;
		damage.damagedDealt = 300;

		target->Hit(damage);
		SetOnCooldown(owner->GetWorld());
	}
}

USnipe* USnipe::CreateAbility(float cooldown, AMech_RPGCharacter* owner) {
	USnipe* ability = NewObject<USnipe>(StaticClass());
	ability->SetCooldown(cooldown);
	ability->owner = owner;
	return ability;
}