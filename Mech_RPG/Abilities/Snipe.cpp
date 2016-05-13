#pragma once
#include "Mech_RPG.h"
#include "Abilities/Snipe.h"
#include "Mech_RPGCharacter.h"


USnipe* USnipe::CreateAbility(float cooldown, AMech_RPGCharacter* owner) {
	USnipe* ability = NewObject<USnipe>(StaticClass());
	ability->SetCooldown(cooldown);
	ability->changeAmount = 0.8F;
	ability->owner = owner;
	ability->affectedTeam = AOEEnums::Enemy;
	ability->AddTag(damageTag, 0.8F);
	return ability;
}

FString USnipe::GetTooltipText()
{
	FString lnBreak = " \n";
	FString damageString = FString::SanitizeFloat(changeAmount * 100);

	return "Snipe" + lnBreak + "Damages target enemy for " + damageString + "% health." + lnBreak + "Cooldown: " + FString::SanitizeFloat(GetCooldown());
}
