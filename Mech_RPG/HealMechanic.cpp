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

void AHealMechanic::SetDead(bool newVal) {
	Super::SetDead(newVal);

	if (IsDead()) {
		GetBoss()->ApplyCrowdControl(EffectEnums::Damage, true);
	}
}
