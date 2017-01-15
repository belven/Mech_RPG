// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "Abilities/DefenceBoost.h"
#include "Characters/Mech_RPGCharacter.h"

bool UDefenceBoost::Activate(class AMech_RPGCharacter* target, FVector targetLocation) {
	SetOnCooldown(owner->GetWorld());
	owner->GetWorld()->GetTimerManager().SetTimer(TimerHandle_DefenceBoostEnded, this, &UDefenceBoost::ResetDefenceBoost, GetCooldown() * 0.5);
	owner->SetDefenceModifier(owner->GetDefenceModifier() + DefenceModifier);

	UE_LOG(AbilitiesLog, Log, TEXT("%d used %s"), owner->GetID(), *GetClass()->GetName());
	return true;
}

UDefenceBoost* UDefenceBoost::CreateAbility(float cooldown, AMech_RPGCharacter* owner, float inDefenceMultiplier) {
	UDefenceBoost* ability = NewObject<UDefenceBoost>(StaticClass());
	ability->SetCooldown(cooldown);
	ability->DefenceModifier = inDefenceMultiplier;
	ability->owner = owner;
	ability->affectedTeam = EAffectedTeam::Ally;
	ability->AddTag(buffTag, inDefenceMultiplier);
	ability->AddTag(needsTargetTag, 0);
	return ability;
}

void  UDefenceBoost::ResetDefenceBoost() {
	owner->SetDefenceModifier(owner->GetDefenceModifier() - DefenceModifier);
}
