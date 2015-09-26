// Fill out your copyright notice in the Description page of Project Settings.

#include "Mech_RPG.h"
#include "TestAOE.h"
#include "Mech_RPGCharacter.h"
#include "AOEHealthChange.h"



void UTestAOE::Activate(AMech_RPGCharacter* target, FVector targetLocation) {
	if (target != NULL) {
		FTempAOESettings settings;
		settings.duration = 5;
		settings.affectedTeam = AOEEnums::Enemy;
		settings.healthChange = damage;
		settings.owner = owner;
		settings.world = owner->GetWorld();
		settings.location = target->GetActorLocation();
		settings.rate = 1;
		settings.radius = 300;
		settings.target = NULL;
		settings.usesTarget = false;
		UAOEHealthChange::CreateTestAOE(settings);
		SetOnCooldown(owner->GetWorld());
	}
}


UTestAOE* UTestAOE::CreateAbility(float cooldown, AMech_RPGCharacter* owner, float inDamage) {
	UTestAOE* ability = NewObject<UTestAOE>(StaticClass());
	ability->SetCooldown(cooldown);
	ability->damage = inDamage;
	ability->owner = owner;
	return ability;
}
