// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "CritBoost.h"

void UCritBoost::Activate(AMech_RPGCharacter * target, FVector targetLocation)
{
	if (owner->GetCurrentWeapon() != nullptr) {
		weaponAffected = owner->GetCurrentWeapon();
		SetOnCooldown(owner->GetWorld());
		owner->GetWorld()->GetTimerManager().SetTimer(TimerHandle_DamageBoostEnded, this, &UCritBoost::ResetCritBoost, GetCooldown() * 0.7);
		weaponAffected->SetCritChance(weaponAffected->GeCritChance() + critMultiplier);
	}
}

UCritBoost * UCritBoost::CreateCritBoost(float cooldown, AMech_RPGCharacter * owner, float inCritMultiplier)
{
	UCritBoost* ability = NewObject<UCritBoost>(StaticClass());
	ability->SetCooldown(cooldown);
	ability->critMultiplier = inCritMultiplier;
	ability->owner = owner;
	return ability;
}

void UCritBoost::ResetCritBoost()
{
	weaponAffected->SetCritChance(weaponAffected->GeCritChance() - critMultiplier);
}
