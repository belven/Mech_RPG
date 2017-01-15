// Fill out your copyright notice in the Description page of Project Settings.

#include "Mech_RPG.h"
#include "Abilities/Stun.h"
#include "Delayed Events/EffectTimer.h"
#include "Characters/Mech_RPGCharacter.h"

bool UStun::Activate(class AMech_RPGCharacter* target, FVector targetLocation) {
	if (UMiscLibrary::IsCharacterAlive(target)) {
		targetCharacter = target;
		TArray<TEnumAsByte<EffectEnums::CrowdControl>> effects;
		effects.Add(EffectEnums::Cast);
		effects.Add(EffectEnums::Attack);
		effects.Add(EffectEnums::Move);
		UEffectTimer::CreateEffectTimer(target, duration, effects);
		SetOnCooldown(owner->GetWorld());

		UE_LOG(AbilitiesLog, Log, TEXT("%d used %s on %d"), owner->GetID(), *GetClass()->GetName(), target->GetID());
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