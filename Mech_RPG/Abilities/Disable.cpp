// Fill out your copyright notice in the Description page of Project Settings.

#include "Mech_RPG.h"
#include "Abilities/Disable.h"
#include "Characters/Mech_RPGCharacter.h"


bool UDisable::Activate(class AMech_RPGCharacter* target, FVector targetLocation) {
	if (target != NULL && !target->IsDead()) {
		targetCharacter = target;
		targetCharacter->ApplyCrowdControl(EffectEnums::Cast, false);
		targetCharacter->ApplyCrowdControl(EffectEnums::Attack, false);
		owner->GetWorld()->GetTimerManager().SetTimer(TimerHandle_DisableEnd, this, &UDisable::DisableEnd, duration);
		SetOnCooldown(owner->GetWorld());

		UE_LOG(AbilitiesLog, Log, TEXT("%d used %s on %d"), owner->GetID(), *GetClass()->GetName(), target->GetID());
		return true;
	}
	return false;
}

UDisable* UDisable::CreateDisable(float cooldown, AMech_RPGCharacter* inOwner, float inDuration) {
	UDisable* ability = NewObject<UDisable>(StaticClass());
	ability->SetCooldown(cooldown);
	ability->owner = inOwner;
	ability->duration = inDuration;
	ability->AddTag(debuffTag, inDuration);
	return ability;
}

FString UDisable::GetTooltipText()
{
	return "Disable" + UMiscLibrary::lnBreak + "Interrupts and prevents target from using abilities and attacking for " + FString::SanitizeFloat(duration) + " seconds" + UMiscLibrary::lnBreak + "Cooldown: " + FString::SanitizeFloat(GetCooldown());
}

void UDisable::DisableEnd() {
	targetCharacter->ApplyCrowdControl(EffectEnums::Cast, true);
	targetCharacter->ApplyCrowdControl(EffectEnums::Attack, true);
}