// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "Abilities/TimedHealthChange.h"
#include "Characters/Mech_RPGCharacter.h"

bool UTimedHealthChange::Activate(class AMech_RPGCharacter* target, FVector targetLocation) {
	if (target != NULL) {
		this->target = target;
		timeLeft = duration;
		owner->GetWorld()->GetTimerManager().SetTimer(TimerHandle_TimeTick, this, &UTimedHealthChange::TimeTick, rate);
		SetOnCooldown(owner->GetWorld());
		return true;
	}
	return false;
}

void UTimedHealthChange::TimeTick() {
	if (UMiscLibrary::IsCharacterAlive(owner) && UMiscLibrary::IsCharacterAlive(target) && timeLeft > 0) {
		timeLeft -= rate;

		FHealthChange healthChange;
		healthChange.damager = owner;
		healthChange.healthChange = GetWeaponHealthChange() * changeAmount;
		healthChange.target = target;
		healthChange.heals = heals;

		target->ChangeHealth(healthChange);
		owner->GetWorld()->GetTimerManager().SetTimer(TimerHandle_TimeTick, this, &UTimedHealthChange::TimeTick, rate);
	}
}

UTimedHealthChange* UTimedHealthChange::CreateTimedHealthChange(AMech_RPGCharacter* inOwner, float cooldown, float inChangeAmount, float inRate, float inDuration, bool inHeals) {
	UTimedHealthChange* ability = NewObject<UTimedHealthChange>(StaticClass());
	ability->owner = inOwner;
	ability->rate = inRate;
	ability->duration = inDuration;
	ability->heals = inHeals;
	ability->affectedTeam = inHeals ? AOEEnums::Ally : AOEEnums::Enemy;
	ability->changeAmount = inChangeAmount;
	ability->SetCooldown(cooldown);

	if (inHeals) {
		ability->AddTag(healTag, inChangeAmount);
	}
	else {
		ability->AddTag(damageTag, inChangeAmount);
	}
	return ability;
}