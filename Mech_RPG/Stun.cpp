// Fill out your copyright notice in the Description page of Project Settings.

#include "Mech_RPG.h"
#include "Stun.h"

void UStun::Activate(AMech_RPGCharacter* target, FVector targetLocation) {

}

void UStun::StunEnd() {

}


UStun* UStun::CreateAbility(float cooldown, AMech_RPGCharacter* owner, float duration) {
	UStun* ability = NewObject<UStun>(StaticClass());
	ability->SetCooldown(cooldown);
	ability->duration = duration;
	ability->owner = owner;
	return ability;
}