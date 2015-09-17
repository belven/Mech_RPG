// Fill out your copyright notice in the Description page of Project Settings.

#include "Mech_RPG.h"
#include "OrbitalStrike.h"
#include "Mech_RPGCharacter.h"
#include "TestAOE_Temp.h"


void UOrbitalStrike::Activate(AMech_RPGCharacter* target, FVector targetLocation) {
	if (target != NULL) {
		FTempAOESettings settings;
		settings.affectedTeam = owner->startingGroupID == 1 ? 0 : 1;
		settings.healthChange = damage;
		settings.owner = owner;
		settings.world = owner->GetWorld();
		settings.rate = 1;
		settings.radius = 700;
		settings.target = target;
		settings.duration = 5;
		settings.usesTarget = true;
		UTestAOE_Temp::CreateTestAOE(settings);
		SetOnCooldown(owner->GetWorld());
	}
}


UOrbitalStrike* UOrbitalStrike::CreateAbility(float cooldown, AMech_RPGCharacter* owner, float inDamage) {
	UOrbitalStrike* ability = NewObject<UOrbitalStrike>(StaticClass());
	ability->SetCooldown(cooldown);
	ability->damage = inDamage;
	ability->owner = owner;
	return ability;
}