// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "DamageIncreaseSkill.h"

float UDamageIncreaseSkill::GetStatBonus(EStatEnum statType)
{
	switch (statType) 
	{
	case EStatEnum::Damage:
		return GetCurrentLevel() * 0.1;
	default:
		return 0.0;
	}
}

UDamageIncreaseSkill* UDamageIncreaseSkill::CreateDamageIncreaseSkill()
{
	UDamageIncreaseSkill* skill = NewObject<UDamageIncreaseSkill>(StaticClass());
	skill->SetName("Explosive Rounds");
	skill->SetStartLevel(0);
	skill->SetMaxLevel(20);
	skill->SetCurrentLevel(20);
	return skill;
}
