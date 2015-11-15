// Fill out your copyright notice in the Description page of Project Settings.

#include "Mech_RPG.h"
#include "Heal.h"



UHeal* UHeal::CreateAbility(float cooldown, AMech_RPGCharacter* owner, float inHealAmount) {
	UHeal* ability = NewObject<UHeal>(StaticClass());
	ability->SetCooldown(cooldown);
	ability->changeAmount = inHealAmount;
	ability->owner = owner;
	ability->affectedTeam = AOEEnums::Ally;
	return ability;
}