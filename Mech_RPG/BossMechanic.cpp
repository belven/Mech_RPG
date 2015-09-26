// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "Mech_RPG.h"
#include "BossMechanic.h"

ABossMechanic::ABossMechanic() : Super(){

}

void ABossMechanic::BeginPlay() {
	for (FConstPawnIterator iter = GetWorld()->GetPawnIterator(); iter; iter++) {
		APawn* pawn = iter->Get();
		if (pawn && pawn != this && pawn->GetDistanceTo(this) <= 1200) {
			ABoss* boss = Cast<ABoss>(pawn);

			if (boss != NULL) {
				SetBoss(boss);
				break;
			}
		}
	}
	Super::BeginPlay();
}

ABoss* ABossMechanic::GetBoss() {
	return boss;
}

void ABossMechanic::SetBoss(ABoss* newVal) {
	boss = newVal;
}