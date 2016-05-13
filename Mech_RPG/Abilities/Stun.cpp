// Fill out your copyright notice in the Description page of Project Settings.

#include "Mech_RPG.h"
#include "Abilities/Stun.h"
#include "Mech_RPGCharacter.h"

bool UStun::Activate(class AMech_RPGCharacter* target, FVector targetLocation) {
	if (UMiscLibrary::IsCharacterAlive(target)) {
		targetCharacter = target;
		targetCharacter->ApplyCrowdControl(EffectEnums::Cast, false);
		targetCharacter->ApplyCrowdControl(EffectEnums::Attack, false);
		targetCharacter->ApplyCrowdControl(EffectEnums::Move, false);
		owner->GetWorld()->GetTimerManager().SetTimer(TimerHandle_StunEnd, this, &UStun::StunEnd, duration);
		SetOnCooldown(owner->GetWorld());
		return true;
	}
	return false;
}

FString UStun::GetTooltipText()
{
	FString durationString = FString::SanitizeFloat(duration);
	return "Stun" + UMiscLibrary::lnBreak + "Stops target from moving and attacking for " + durationString + UMiscLibrary::lnBreak + "Cooldown: " + FString::SanitizeFloat(GetCooldown());
}

void UStun::StunEnd() {
	targetCharacter->ApplyCrowdControl(EffectEnums::Cast, true);
	targetCharacter->ApplyCrowdControl(EffectEnums::Attack, true);
	targetCharacter->ApplyCrowdControl(EffectEnums::Move, true);
}

UStun* UStun::CreateAbility(float cooldown, AMech_RPGCharacter* owner, float duration) {
	UStun* ability = NewObject<UStun>(StaticClass());
	ability->SetCooldown(cooldown);
	ability->duration = duration;
	ability->owner = owner;
	ability->AddTag(debuffTag, duration);
	return ability;
}