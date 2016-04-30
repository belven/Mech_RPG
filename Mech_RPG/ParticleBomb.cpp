// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "ParticleBomb.h"
#include "Mech_RPGCharacter.h"


bool UParticleBomb::Activate(class AMech_RPGCharacter* target, FVector targetLocation) {
	if (!targetLocation.IsZero()) {
		FTempAOESettings settings;
		settings.affectedTeam = AOEEnums::Enemy;
		settings.healthChange = damage;
		settings.owner = owner;
		settings.world = owner->GetWorld();
		settings.rate = 1;
		settings.radius = 300;
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


UParticleBomb* UParticleBomb::CreateAbility(float cooldown, AMech_RPGCharacter* owner, float inDamage) {
	UParticleBomb* ability = NewObject<UParticleBomb>(StaticClass());
	ability->SetCooldown(cooldown);
	ability->damage = inDamage;
	ability->owner = owner;
	ability->AddTag(aoeTag, 300);
	ability->AddTag(damageTag, inDamage);
	return ability;
}

