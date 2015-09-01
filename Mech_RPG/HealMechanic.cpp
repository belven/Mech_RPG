// Fill out your copyright notice in the Description page of Project Settings.

#include "Mech_RPG.h"
#include "HealMechanic.h"

void AHealMechanic::Tick(float DeltaTime) {
	if (!IsDead()) {
		if (GetBoss() && !GetBoss()->IsDead() && GetBoss()->GetHealth() < GetBoss()->GetMaxHealth()) {
			GetBoss()->SetHealth(GetBoss()->GetHealth() + healAmount * DeltaTime);
		}
	}
	else {
		Destroy();
	}
}