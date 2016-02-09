// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "TargetedHealthChange.h"
#include "Mech_RPGCharacter.h"

void UTargetedHealthChange::Activate(AMech_RPGCharacter* target, FVector targetLocation) {
	if (UMiscLibrary::IsCharacterAlive(target)) {
		FHealthChange healthChange;
		float tempChangeAmount = healthChange.heals = affectedTeam == AOEEnums::Ally;

		tempChangeAmount = (changeAmount <= 2.0F) ? target->GetMaxHealth() * changeAmount : changeAmount;

		healthChange.damager = owner;
		healthChange.target = target;
		healthChange.weaponUsed = nullptr;
		healthChange.healthChange = tempChangeAmount;

		target->ChangeHealth(healthChange);
		SetOnCooldown(owner->GetWorld());
	}
}

UTargetedHealthChange* UTargetedHealthChange::CreateAbility(float cooldown, AMech_RPGCharacter* owner, float inChangeAmount, AOEEnums::AffectedTeam team) {
	UTargetedHealthChange* ability = NewObject<UTargetedHealthChange>(StaticClass());
	ability->SetCooldown(cooldown);
	ability->changeAmount = inChangeAmount;
	ability->owner = owner;
	ability->affectedTeam = team;
	return ability;
}