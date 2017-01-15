// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "Abilities/AoEHeal.h"
#include "Characters/Mech_RPGCharacter.h"

bool UAoEHeal::Activate(class AMech_RPGCharacter* target, FVector targetLocation) {
	if (!targetLocation.IsZero()) {
		FTempAOESettings settings;
		settings.affectedTeam = GetAffectedTeam();
		settings.healthChange = GetWeaponHealthChange() * healAmount;
		settings.owner = owner;
		settings.world = owner->GetWorld();
		settings.rate = 0.5F;
		settings.radius = 700;
		settings.location = targetLocation;
		settings.damageType = EDamageType::Blast;
		settings.duration = 3.0F;
		settings.usesTarget = false;
		settings.heals = true;
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

UAoEHeal* UAoEHeal::CreateAbility(float cooldown, AMech_RPGCharacter* owner, float inHealAmount) {
	UAoEHeal* ability = NewObject<UAoEHeal>(StaticClass());
	ability->SetCooldown(cooldown);
	ability->healAmount = inHealAmount;
	ability->affectedTeam = EAffectedTeam::Ally;
	ability->owner = owner;
	ability->AddTag(healTag, inHealAmount);
	ability->AddTag(aoeTag, 700);
	return ability;
}

FString UAoEHeal::GetTooltipText()
{
	FString lnBreak = " \n";
	FString healString = FString::SanitizeFloat(GetWeaponHealthChange() * healAmount);
	return "AoE Heal" + lnBreak + "Heals all allies in a " + FString::FromInt(700) + " radius, for " + healString + " health." + lnBreak + "Cooldown: " + FString::SanitizeFloat(GetCooldown());
}

