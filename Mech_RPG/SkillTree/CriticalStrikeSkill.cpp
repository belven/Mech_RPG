// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "CriticalStrikeSkill.h"


float UCriticalStrikeSkill::GetStatBonus(EStatEnum statType)
{
	switch (statType)
	{
	case EStatEnum::CritChance:
		return GetCurrentLevel() * 0.1;
	default:
		return 0.0;
	}
}

UCriticalStrikeSkill* UCriticalStrikeSkill::CreateCriticalStrikeSkill()
{
	UCriticalStrikeSkill* skill = NewObject<UCriticalStrikeSkill>(StaticClass());
	skill->SetName("Precision Scanner");
	skill->SetStartLevel(0);
	skill->SetMaxLevel(20);
	skill->SetCurrentLevel(1);
	return skill;
}
