// Fill out your copyright notice in the Description page of Project Settings.

#include "Mech_RPG.h"
#include "TestAOE.h"
#include "Mech_RPGCharacter.h"
#include "AOEVolume.h"



void UTestAOE::Activate(AMech_RPGCharacter* target, FVector targetLocation) {
	if (target != NULL) {
		FAOESettings settings;
		settings.duration = 5;
		settings.inAffectedTeam = owner->startingGroupID == 1 ? 0 : 1;
		settings.inHealthChange = damage;
		settings.owner = owner;
		settings.world = owner->GetWorld();
		settings.Location = target->GetActorLocation();
		AAOEVolume::CreateAAOEVolume(settings);
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
