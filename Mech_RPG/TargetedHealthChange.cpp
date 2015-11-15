// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "TargetedHealthChange.h"
#include "Mech_RPGCharacter.h"

void UTargetedHealthChange::Activate(AMech_RPGCharacter* target, FVector targetLocation) {
	if (UMiscLibrary::IsCharacterAlive(target)) {
		FHealthChange damage;
		float damageToDo = affectedTeam == AOEEnums::Ally ? -changeAmount : changeAmount;

		damageToDo = (damageToDo <= 2.0F) ? target->GetMaxHealth() * damageToDo : damageToDo;

		damage.damager = owner;
		damage.target = target;
		damage.weaponUsed = NULL;
		damage.healthChange = damageToDo;

		target->ChangeHealth(damage);
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