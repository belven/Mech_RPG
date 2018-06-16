// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "CriticalStrikeSkill.h"


TMap<EStatEnum, float> UCriticalStrikeSkill::GetStatBonuses()
{
	TMap<EStatEnum, float> bonuses;
	bonuses.Add(EStatEnum::CritChance, 0.5);
	return bonuses;
}
