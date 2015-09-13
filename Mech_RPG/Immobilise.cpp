// Fill out your copyright notice in the Description page of Project Settings.

#include "Mech_RPG.h"
#include "Immobilise.h"


void UImmobilise::Activate(AMech_RPGCharacter* target, FVector targetLocation) {
	if (target != NULL && !target->IsDead() && target->GetController()) {
		SetOnCooldown(owner->GetWorld());
		owner->GetWorld()->GetTimerManager().SetTimer(TimerHandle_ImmobiliseEnded, this, &UImmobilise::ResetImmobilise, duration);
		target->GetCanMove()++;
		target->GetController()->StopMovement();
		lastTarget = target;
	}
}

UImmobilise* UImmobilise::CreateAbility(float cooldown, AMech_RPGCharacter* owner, float duration) {
	UImmobilise* ability = NewObject<UImmobilise>(StaticClass());
	ability->SetCooldown(cooldown);
	ability->duration = duration;
	ability->owner = owner; 
	return ability;
}

void UImmobilise::ResetImmobilise() {
	if (lastTarget != NULL) {
		lastTarget->GetCanMove()--;
	}
}