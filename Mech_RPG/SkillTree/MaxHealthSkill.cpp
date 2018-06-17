// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "MaxHealthSkill.h"

float UMaxHealthSkill::GetStatBonus(EStatEnum statType)
{
	switch (statType)
	{
	case EStatEnum::Health:
		return GetCurrentLevel() * 0.1;
	default:
		return 0.0;
	}
}

UMaxHealthSkill* UMaxHealthSkill::CreateMaxHealthSkill()
{
	UMaxHealthSkill* skill = NewObject<UMaxHealthSkill>(StaticClass());
	skill->SetName("Fortify");
	skill->SetStartLevel(0);
	skill->SetMaxLevel(20);
	skill->SetCurrentLevel(0);
	return skill;
}

FString UMaxHealthSkill::GetTooltipText()
{
	FString tooltip = "Increases maximum health by ";
	tooltip += FString::SanitizeFloat(GetCurrentLevel() * 0.1 * 100);
	tooltip += "%";
	return tooltip;
}
