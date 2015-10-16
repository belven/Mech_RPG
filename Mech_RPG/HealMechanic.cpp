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

	if (IsDead()) {
		GetBoss()->ApplyCrowdControl(EffectEnums::Damage, true);
	}
}