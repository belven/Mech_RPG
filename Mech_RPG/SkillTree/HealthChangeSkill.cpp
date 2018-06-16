// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "HealthChangeSkill.h"

TMap<EStatEnum, float> UHealthChangeSkill::GetStatBonuses()
{
	TMap<EStatEnum, float> bonuses;
	bonuses.Add(EStatEnum::Damage, 0.5);
	return bonuses;
}
