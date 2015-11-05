// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "DefenceBoost.h"

void UDefenceBoost::Activate(AMech_RPGCharacter* target, FVector targetLocation) {
	SetOnCooldown(owner->GetWorld());
	owner->GetWorld()->GetTimerManager().SetTimer(TimerHandle_DefenceBoostEnded, this, &UDefenceBoost::ResetDefenceBoost, GetCooldown() * 0.5);
	owner->SetDefenceModifier(owner->GetDefenceModifier() + DefenceModifier);
}

UDefenceBoost* UDefenceBoost::CreateAbility(float cooldown, AMech_RPGCharacter* owner, float inDefenceMultiplier) {
	UDefenceBoost* ability = NewObject<UDefenceBoost>(StaticClass());
	ability->SetCooldown(cooldown);
	ability->DefenceModifier = inDefenceMultiplier;
	ability->owner = owner;
	ability->affectedTeam = AOEEnums::Ally;
	return ability;
}

void  UDefenceBoost::ResetDefenceBoost() {
	owner->SetDefenceModifier(owner->GetDefenceModifier() - DefenceModifier);
}
