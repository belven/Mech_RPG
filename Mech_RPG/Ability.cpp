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

bool UAbility::GetOnCooldown(){
	return onCooldown;
}

void UAbility::SetCooldown(float newCooldown){
	cooldown = newCooldown;
}

void UAbility::ResetOnCooldown(){
	onCooldown = false;
}

UAbility* UAbility::CreateAbility(){
	return NewObject<UAbility>(UAbility::StaticClass());
}

//void UAbility::Activate(AShip* owner){
//	static TArray<AActor*>& tempActors = *new TArray<AActor*>();
//
//	owner->GetAoeDetection()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
//	owner->GetAoeDetection()->GetOverlappingActors(tempActors, AShip::StaticClass());
//
//	for (AActor* temp : tempActors)
//	{
//		AShip* ship = Cast<AShip>(temp);
//		if (ship && ship != owner)
//		{
//			ship->TakeDamage(10);
//		}
//	}
//
//	owner->GetAoeDetection()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
//	tempActors.Empty();
//}