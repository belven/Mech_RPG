// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "Characters/Mech_RPGCharacter.h"
#include "Abilities/CritBoost.h"
#include "Weapons/Weapon.h"

bool UCritBoost::Activate(class AMech_RPGCharacter* target, FVector targetLocation)
{
	if (owner->GetCurrentWeapon() != nullptr && count <= 0) {
		weaponAffected = owner->GetCurrentWeapon();
		SetOnCooldown(owner->GetWorld());
		weaponAffected->SetCritChance(weaponAffected->GetCritChance() + critMultiplier);
		weaponAffected->OnFire.AddDynamic(this, &UCritBoost::OwnerFired);
		count = 10;
		return true;
	}
	return false;
}

void UCritBoost::OwnerFired() {
	count--;

	if (count <= 0) {
		weaponAffected->OnFire.RemoveDynamic(this, &UCritBoost::OwnerFired);
		ResetCritBoost();
	}
}

FString UCritBoost::GetTooltipText()
{
	FString lnBreak = " \n";
	FString critString = FString::SanitizeFloat(critMultiplier < 2 ? critMultiplier * 100 : critMultiplier);
	return "Critical Boost" + lnBreak + "Increases weapons crit chance by " + critString + "%" + lnBreak + "Cooldown: " + FString::SanitizeFloat(GetCooldown());
}

UCritBoost * UCritBoost::CreateCritBoost(float cooldown, AMech_RPGCharacter * owner, float inCritMultiplier)
{
	UCritBoost* ability = NewObject<UCritBoost>(StaticClass());
	ability->SetCooldown(cooldown);
	ability->critMultiplier = inCritMultiplier;
	ability->owner = owner;
	ability->AddTag(buffTag, inCritMultiplier);
	ability->AddTag(needsTargetTag, 0);
	return ability;
}

void UCritBoost::ResetCritBoost()
{
	weaponAffected->SetCritChance(weaponAffected->GetCritChance() - critMultiplier);
}
