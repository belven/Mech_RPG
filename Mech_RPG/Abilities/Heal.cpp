// Fill out your copyright notice in the Description page of Project Settings.

#include "Mech_RPG.h"
#include "Abilities/Heal.h"
#include "Characters/Mech_RPGCharacter.h"

UHeal* UHeal::CreateAbility(float cooldown, AMech_RPGCharacter* owner, float inHealAmount) {
	UHeal* ability = NewObject<UHeal>(StaticClass());
	ability->SetCooldown(cooldown);
	ability->changeAmount = inHealAmount;
	ability->owner = owner;
	ability->affectedTeam = EAffectedTeam::Ally;
	ability->AddTag(healTag, inHealAmount);
	return ability;
}

FString UHeal::GetTooltipText()
{
	FString lnBreak = " \n";
	FString healString = FString::FromInt(GetWeaponHealthChange() * changeAmount);

	return "Heal" + lnBreak + "Heals target ally for " + healString + " health." + lnBreak + "Cooldown: " + FString::SanitizeFloat(GetCooldown());
}