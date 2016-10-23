// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "Mech_RPG.h"
#include "AOEHealthChange.h"
#include "Characters/Mech_RPGCharacter.h"
#include "OrbitalStrike.h"

bool UOrbitalStrike::Activate(class AMech_RPGCharacter* target, FVector targetLocation) {
	FTempAOESettings settings;
	settings.affectedTeam = AOEEnums::Enemy;
	settings.owner = owner;
	settings.healthChange = GetWeaponHealthChange() *  damage;
	settings.world = owner->GetWorld();
	settings.rate = 1.5;
	settings.radius = 425;
	settings.target = target;
	settings.duration = settings.rate * 3;
	settings.usesTarget = false;
	settings.location = targetLocation;
	AAOEHealthChange::CreateAOEHealthChange(settings);
	SetOnCooldown(owner->GetWorld());
	return true;
}

UOrbitalStrike* UOrbitalStrike::CreateAbility(float cooldown, AMech_RPGCharacter* owner, float inDamage) {
	UOrbitalStrike* ability = NewObject<UOrbitalStrike>(StaticClass());
	ability->SetCooldown(cooldown);
	ability->damage = inDamage;
	ability->owner = owner;
	ability->AddTag(aoeTag, 425);
	ability->AddTag(damageTag, inDamage);
	ability->AddTag(needsTargetTag, 0);
	return ability;
}