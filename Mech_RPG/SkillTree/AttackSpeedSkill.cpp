// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "AttackSpeedSkill.h"

float UAttackSpeedSkill::GetStatBonus(EStatEnum statType)
{
	switch (statType)
	{
	case EStatEnum::AttackSpeed:
		return GetCurrentLevel() * 0.1;
	default:
		return 0.0;
	}
}

UAttackSpeedSkill* UAttackSpeedSkill::CreateAttackSpeedSkill()
{
	UAttackSpeedSkill* skill = NewObject<UAttackSpeedSkill>(StaticClass());
	skill->SetName("Quicken");
	skill->SetStartLevel(0);
	skill->SetMaxLevel(20);
	skill->SetCurrentLevel(0);
	return skill;
}

FString UAttackSpeedSkill::GetTooltipText()
{
	FString tooltip = "Increases attack speed by ";
	tooltip += FString::SanitizeFloat(GetCurrentLevel() * 0.1 * 100);
	tooltip += "%";
	return tooltip;
}
