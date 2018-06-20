// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "HealingReceivedSkill.h"

float UHealingReceivedSkill::GetStatBonus(EStatEnum statType)
{
	return 0.0;
}

UHealingReceivedSkill* UHealingReceivedSkill::CreateHealingRecievedSkill()
{
	UHealingReceivedSkill* skill = NewObject<UHealingReceivedSkill>(StaticClass());
	skill->SetName("Healing Received");
	skill->SetStartLevel(0);
	skill->SetMaxLevel(20);
	skill->SetCurrentLevel(0);
	return skill;
}

FString UHealingReceivedSkill::GetTooltipText()
{
	FString tooltip = "Increases healing Received by ";
	tooltip += FString::SanitizeFloat(GetCurrentLevel() * 0.1 * 100);
	tooltip += "%";
	return tooltip;
}

void UHealingReceivedSkill::PreHealthChanged(FHealthChange& healthChange)
{
	if (healthChange.heals) {
		float bonus = 1 + (GetCurrentLevel() * 0.1);
		healthChange.changeAmount *= bonus;
	}
}
