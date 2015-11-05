// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "TimedHealthChange.h"

void UTimedHealthChange::Activate(AMech_RPGCharacter* target, FVector targetLocation) {
	if (target != NULL) {
		this->target = target;
		owner->GetWorld()->GetTimerManager().SetTimer(TimerHandle_AbilityOffCooldown, this, &UTimedHealthChange::TimeTick, 0.1F);
	}
}

void UTimedHealthChange::TimeTick() {

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
	return ability;
}