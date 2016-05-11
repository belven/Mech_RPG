// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "Grenade.h"
#include "Mech_RPGCharacter.h"

bool UGrenade::Activate(class AMech_RPGCharacter* target, FVector targetLocation) {
	if (!targetLocation.IsZero()) {
		FTempAOESettings settings;
		settings.affectedTeam = AOEEnums::Enemy;
		settings.healthChange = damage;
		settings.owner = owner;
		settings.world = owner->GetWorld();
		settings.rate = 1;
		settings.radius = 450;
		settings.location = targetLocation;
		settings.damageType = DamageEnums::Blast;
		settings.duration = 1;
		settings.usesTarget = false;
		AAOEHealthChange::CreateAOEHealthChange(settings);
		SetOnCooldown(owner->GetWorld());
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