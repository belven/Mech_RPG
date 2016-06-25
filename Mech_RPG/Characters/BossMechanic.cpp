// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "Mech_RPG.h"
#include "BossMechanic.h"

ABossMechanic::ABossMechanic() : Super() {

}

void ABossMechanic::BeginPlay() {
	Super::BeginPlay();

	for (AMech_RPGCharacter* character : UMiscLibrary::GetCharactersInRange(1200, this)) {
		if (character != nullptr && character->GetClass()->IsChildOf(ABoss::StaticClass()) && character->CompareGroup(this)) {
			ABoss* boss = Cast<ABoss>(character);
			SetBoss(boss);
			SetGroup(GetBoss()->GetGroup());
			break;
		}
	}
}

ABoss* ABossMechanic::GetBoss() {
	return boss;
}

void ABossMechanic::SetBoss(ABoss* newVal) {
	boss = newVal;
}

void ABossMechanic::CreatePresetRole(TEnumAsByte<GroupEnums::Role> inRole, int32 grade, int32 quaility)
{
	AMech_RPGCharacter::CreatePresetRole(inRole, grade, quaility);

	SetMaxHealth(GetMaxHealth() * 1.7);
	SetHealth(GetMaxHealth());
}