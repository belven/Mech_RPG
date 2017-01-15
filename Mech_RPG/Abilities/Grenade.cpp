// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "Abilities/Grenade.h"
#include "Characters/Mech_RPGCharacter.h"

bool UGrenade::Activate(class AMech_RPGCharacter* target, FVector targetLocation) {
	if (!targetLocation.IsZero()) {
		FTempAOESettings settings;
		settings.affectedTeam = EAffectedTeam::Enemy;
		settings.healthChange = GetWeaponHealthChange() * damage;
		settings.owner = owner;
		settings.world = owner->GetWorld();
		settings.rate = 1;
		settings.radius = 450;
		settings.location = targetLocation;
		settings.damageType = EDamageType::Blast;
		settings.duration = 1;
		settings.usesTarget = false;
		AAOEHealthChange::CreateAOEHealthChange(settings);
		SetOnCooldown(owner->GetWorld());
		
		if (target != nullptr) {
			UE_LOG(AbilitiesLog, Log, TEXT("%d used %s on %d"), owner->GetID(), *GetClass()->GetName(), target->GetID());
		}
		else {
			UE_LOG(AbilitiesLog, Log, TEXT("%d used %s on %s"), owner->GetID(), *GetClass()->GetName(), *targetLocation.ToString());
		}

		return true;
	}
	return false;
}

UGrenade* UGrenade::CreateAbility(float cooldown, AMech_RPGCharacter* owner, float inDamage) {
	UGrenade* ability = NewObject<UGrenade>(StaticClass());
	ability->SetCooldown(cooldown);
	ability->damage = inDamage;
	ability->owner = owner;
	ability->AddTag(damageTag, inDamage);
	ability->AddTag(aoeTag, 450);
	return ability;
}

FString UGrenade::GetTooltipText()
{
	FString lnBreak = " \n";
	FString damageString = FString::SanitizeFloat(GetWeaponHealthChange() * damage);

	return "Grenade" + lnBreak + "Damages all enemies in a " + FString::FromInt(450) + " radius, for " + damageString + " health." + lnBreak + "Cooldown: " + FString::SanitizeFloat(GetCooldown());
}