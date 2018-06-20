// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "ResistanceBoostSkill.h"


float UResistanceBoostSkill::GetStatBonus(EStatEnum statType)
{
	switch (statType)
	{
	case EStatEnum::Defense:
		return GetCurrentLevel() * 0.1;
	default:
		return 0.0;
	}
}

UResistanceBoostSkill* UResistanceBoostSkill::CreateResistanceBoostSkill()
{
	UResistanceBoostSkill* skill = NewObject<UResistanceBoostSkill>(StaticClass());
	skill->SetName("Reinforce");
	skill->SetStartLevel(0);
	skill->SetMaxLevel(20);
	skill->SetCurrentLevel(0);
	return skill;
}

FString UResistanceBoostSkill::GetTooltipText()
{
	FString tooltip = "Increases defense by ";
	tooltip += FString::SanitizeFloat(GetCurrentLevel() * 0.1 * 100);
	tooltip += "%";
	return tooltip;
}
