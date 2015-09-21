// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "Grenade.h"

void UGrenade::Activate(AMech_RPGCharacter* target, FVector targetLocation) {
	if (target != NULL) {
		FTempAOESettings settings;
		settings.affectedTeam = owner->startingGroupID == 1 ? 0 : 1;
		settings.healthChange = damage;
		settings.owner = owner;
		settings.world = owner->GetWorld();
		settings.rate = 1;
		settings.radius = 400;
		settings.target = target;
		settings.duration = 1;
		settings.usesTarget = true;
		UTestAOE_Temp::CreateTestAOE(settings);
		SetOnCooldown(owner->GetWorld());
	}
}


UGrenade* UGrenade::CreateAbility(float cooldown, AMech_RPGCharacter* owner, float inDamage) {
	UGrenade* ability = NewObject<UGrenade>(StaticClass());
	ability->SetCooldown(cooldown);
	ability->damage = inDamage;
	ability->owner = owner;
	return ability;
}