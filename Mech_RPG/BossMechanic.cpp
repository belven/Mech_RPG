// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "Mech_RPG.h"
#include "BossMechanic.h"

ABossMechanic::ABossMechanic() : Super() {

}

void ABossMechanic::BeginPlay() {
	Super::BeginPlay();

	for (AMech_RPGCharacter* character : UMiscLibrary::GetCharactersInRange(1200, this)) {
		if (character != NULL && character->GetClass()->IsChildOf(ABoss::StaticClass())) {
			ABoss* boss = Cast<ABoss>(character);
			SetBoss(boss);
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