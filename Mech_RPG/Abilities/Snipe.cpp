#pragma once
#include "Mech_RPG.h"
#include "Abilities/Snipe.h"
#include "Characters/Mech_RPGCharacter.h"


USnipe* USnipe::CreateAbility(float cooldown, AMech_RPGCharacter* owner) {
	USnipe* ability = NewObject<USnipe>(StaticClass());
	ability->SetCooldown(cooldown);
	ability->owner = owner;
	ability->changeAmount = 20.0F;
	ability->affectedTeam = AOEEnums::Enemy;
	ability->AddTag(damageTag, 0.8F);
	return ability;
}

FString USnipe::GetTooltipText()
{
	FString lnBreak = " \n";
	FString damageString = FString::SanitizeFloat(GetWeaponHealthChange() * changeAmount);
	return "Snipe" + lnBreak + "Damages target enemy for " + damageString + "% health." + lnBreak + "Cooldown: " + FString::SanitizeFloat(GetCooldown());
}
