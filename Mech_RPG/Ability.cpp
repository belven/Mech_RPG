// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "Mech_RPG.h"
#include "Ability.h"
#include "Mech_RPGCharacter.h"

void UAbility::SetOnCooldown(UWorld* const World){
	onCooldown = true;
	World->GetTimerManager().SetTimer(TimerHandle_ShotTimerExpired, this, &UAbility::ResetOnCooldown, GetCooldown() > 0 ? GetCooldown() : 0.2);
}

float UAbility::GetCooldown(){
	return cooldown;
}

bool UAbility::OnCooldown(){
	return onCooldown;
}

void UAbility::SetCooldown(float newCooldown){
	cooldown = newCooldown;
}

void UAbility::ResetOnCooldown(){
	onCooldown = false;
}

UAbility* UAbility::CreateAbility(UClass* classType){
	return ConstructObject<UAbility>(classType);
}