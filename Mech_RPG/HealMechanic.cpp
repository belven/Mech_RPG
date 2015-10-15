// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "Mech_RPG.h"
#include "HealMechanic.h"

AHealMechanic::AHealMechanic() : Super() {

}

void AHealMechanic::BeginPlay() {
	Super::BeginPlay();
	if (GetBoss()) {
		GetBoss()->ApplyCrowdControl(EffectEnums::Damage, false);
	}
}

void AHealMechanic::Tick(float DeltaTime) {
	AMech_RPGCharacter::Tick(DeltaTime);

	if (!IsDead()) {
		if (GetBoss() && !GetBoss()->IsDead() && GetBoss()->GetHealth() < GetBoss()->GetMaxHealth()) {
			GetBoss()->ApplyCrowdControl(EffectEnums::Damage, true);
		}
	}
	else {
		GetBoss()->ApplyCrowdControl(EffectEnums::Damage, true);
	}
}