// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "CritBoost.h"
#include "Weapon.h"

void UCritBoost::Activate(AMech_RPGCharacter * target, FVector targetLocation)
{
	if (owner->GetCurrentWeapon() != nullptr && count <= 0) {
		weaponAffected = owner->GetCurrentWeapon();
		SetOnCooldown(owner->GetWorld());
		weaponAffected->SetCritChance(weaponAffected->GetCritChance() + critMultiplier);
		weaponAffected->OnFire.AddDynamic(this, &UCritBoost::OwnerFired);
		count = 10;
	}
}

void UCritBoost::OwnerFired() {
	count--;

	if (count <= 0) {
		weaponAffected->OnFire.RemoveDynamic(this, &UCritBoost::OwnerFired);
		ResetCritBoost();
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
	weaponAffected->SetCritChance(weaponAffected->GetCritChance() - critMultiplier);
}
