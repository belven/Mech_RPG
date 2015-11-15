#pragma once
#include "Mech_RPG.h"
#include "Snipe.h"
#include "Mech_RPGCharacter.h"


USnipe* USnipe::CreateAbility(float cooldown, AMech_RPGCharacter* owner) {
	USnipe* ability = NewObject<USnipe>(StaticClass());
	ability->SetCooldown(cooldown);
	ability->changeAmount = 0.8F;
	ability->owner = owner;
	ability->affectedTeam = AOEEnums::Enemy;
	return ability;
}