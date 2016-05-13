// Fill out your copyright notice in the Description page of Project Settings.

#include "Mech_RPG.h"
#include "Abilities/Immobilise.h"
#include "Mech_RPGCharacter.h"


bool UImmobilise::Activate(class AMech_RPGCharacter* target, FVector targetLocation) {
	if (target != NULL && !target->IsDead() && target->GetController()) {
		SetOnCooldown(owner->GetWorld());
		owner->GetWorld()->GetTimerManager().SetTimer(TimerHandle_ImmobiliseEnded, this, &UImmobilise::ResetImmobilise, duration);
		target->GetCanMove()++;
		target->GetController()->StopMovement();
		lastTarget = target;
		return true;
	}
	return false;
}

UImmobilise* UImmobilise::CreateAbility(float cooldown, AMech_RPGCharacter* owner, float duration) {
	UImmobilise* ability = NewObject<UImmobilise>(StaticClass());
	ability->SetCooldown(cooldown);
	ability->duration = duration;
	ability->owner = owner;
	ability->AddTag(debuffTag, duration);
	return ability;
}

FString UImmobilise::GetTooltipText()
{
	return "Immobilise" + UMiscLibrary::lnBreak + "Prevents target from using moving for " + FString::SanitizeFloat(duration) + " seconds" + UMiscLibrary::lnBreak + "Cooldown: " + FString::SanitizeFloat(GetCooldown());
}

void UImmobilise::ResetImmobilise() {
	if (lastTarget != NULL) {
		lastTarget->ApplyCrowdControl(EffectEnums::Move, true);
	}
}