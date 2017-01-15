// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "Abilities/ParticleBomb.h"
#include "Characters/Mech_RPGCharacter.h"


bool UParticleBomb::Activate(class AMech_RPGCharacter* target, FVector targetLocation) {
	if (!targetLocation.IsZero()) {
		FTempAOESettings settings;
		settings.affectedTeam = EAffectedTeam::Enemy;
		settings.healthChange = GetWeaponHealthChange() *  damage;
		settings.owner = owner;
		settings.world = owner->GetWorld();
		settings.rate = 1;
		settings.radius = 300;
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


UParticleBomb* UParticleBomb::CreateAbility(float cooldown, AMech_RPGCharacter* owner, float inDamage) {
	UParticleBomb* ability = NewObject<UParticleBomb>(StaticClass());
	ability->SetCooldown(cooldown);
	ability->damage = inDamage;
	ability->owner = owner;
	ability->AddTag(aoeTag, 300);
	ability->AddTag(damageTag, inDamage);
	return ability;
}

