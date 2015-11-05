// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "AoEHeal.h"


void UAoEHeal::Activate(AMech_RPGCharacter* target, FVector targetLocation) {
	if (!targetLocation.IsZero()) {
		FTempAOESettings settings;
		settings.affectedTeam = GetAffectedTeam();
		settings.healthChange = healAmount;
		settings.owner = owner;
		settings.world = owner->GetWorld();
		settings.rate = 1;
		settings.radius = 700;
		settings.location = targetLocation;
		settings.damageType = DamageEnums::Blast;
		settings.duration = 1;
		settings.usesTarget = false;
		UAOEHealthChange::CreateTestAOE(settings);
		SetOnCooldown(owner->GetWorld());
	}
}

UAoEHeal* UAoEHeal::CreateAbility(float cooldown, AMech_RPGCharacter* owner, float inHealAmount) {
	UAoEHeal* ability = NewObject<UAoEHeal>(StaticClass());
	ability->SetCooldown(cooldown);
	ability->healAmount = inHealAmount;
	ability->affectedTeam = AOEEnums::Ally;
	ability->owner = owner;
	return ability;
}