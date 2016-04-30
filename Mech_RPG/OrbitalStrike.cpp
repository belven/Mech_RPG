// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "Mech_RPG.h"
#include "AOEHealthChange.h"
#include "Mech_RPGCharacter.h"


bool UOrbitalStrike::Activate(class AMech_RPGCharacter* target, FVector targetLocation) {
	if (target != NULL) {
		FTempAOESettings settings;
		settings.affectedTeam = AOEEnums::Enemy;
		settings.healthChange = damage;
		settings.owner = owner;
		settings.world = owner->GetWorld();
		settings.rate = 1.75;
		settings.radius = 500;
		settings.target = target;
		settings.duration = settings.rate * 5;
		settings.usesTarget = true;
		AAOEHealthChange::CreateAOEHealthChange(settings);
		SetOnCooldown(owner->GetWorld());
		return true;
	}
	return false;
}

UOrbitalStrike* UOrbitalStrike::CreateAbility(float cooldown, AMech_RPGCharacter* owner, float inDamage) {
	UOrbitalStrike* ability = NewObject<UOrbitalStrike>(StaticClass());
	ability->SetCooldown(cooldown);
	ability->damage = inDamage;
	ability->owner = owner;
	ability->AddTag(aoeTag, 500);
	ability->AddTag(damageTag, inDamage);
	return ability;
}