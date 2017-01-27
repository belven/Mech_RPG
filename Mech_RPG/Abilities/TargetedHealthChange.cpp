// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "Abilities/TargetedHealthChange.h"
#include "Characters/Mech_RPGCharacter.h"

bool UTargetedHealthChange::Activate(class AMech_RPGCharacter* target, FVector targetLocation) {
	if (UMiscLibrary::IsCharacterAlive(target)) {
		FHealthChange healthChange;
		float tempChangeAmount = 0.0F;
		healthChange.heals = affectedTeam == EAffectedTeam::Ally;

		tempChangeAmount = GetWeaponHealthChange() * changeAmount;

		healthChange.manipulator = owner;
		healthChange.target = target;
		healthChange.weaponUsed = nullptr;
		healthChange.changeAmount = tempChangeAmount;

		target->ChangeHealth(healthChange);
		SetOnCooldown(owner->GetWorld());

		UE_LOG(AbilitiesLog, Log, TEXT("%d used %s on %d"), owner->GetID(), *GetClass()->GetName(), target->GetID());
		return true;
	}
	return false;
}

UTargetedHealthChange* UTargetedHealthChange::CreateAbility(float cooldown, AMech_RPGCharacter* owner, float inChangeAmount, EAffectedTeam team) {
	UTargetedHealthChange* ability = NewObject<UTargetedHealthChange>(StaticClass());
	ability->SetCooldown(cooldown);
	ability->changeAmount = inChangeAmount;
	ability->owner = owner;
	ability->affectedTeam = team;

	if (team == EAffectedTeam::Ally) {
		ability->AddTag(healTag, inChangeAmount);
	}
	else {
		ability->AddTag(damageTag, inChangeAmount);
	}
	return ability;
}