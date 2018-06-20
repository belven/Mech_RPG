// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "ShotgunDamageSkill.h"

float UShotgunDamageSkill::GetStatBonus(EStatEnum statType)
{
	return 0;
}

UShotgunDamageSkill* UShotgunDamageSkill::CreateShotgunDamageSkill()
{
	UShotgunDamageSkill* skill = NewObject<UShotgunDamageSkill>(StaticClass());
	skill->SetName("Shotgun Power");
	skill->SetStartLevel(0);
	skill->SetMaxLevel(20);
	skill->SetCurrentLevel(0);
	return skill;
}

FString UShotgunDamageSkill::GetTooltipText()
{
	FString tooltip = "Increases shotgun damage by ";
	tooltip += FString::SanitizeFloat(GetCurrentLevel() * 0.1 * 100);
	tooltip += "%";
	return tooltip;
}

void UShotgunDamageSkill::OwnerPreChangedOthersHealth(FHealthChange& healthChange)
{
	if (healthChange.weaponUsed != nullptr && mCheckWeaponUsed(healthChange, UShotgun)) 
	{
		healthChange.changeAmount *= 1 + (GetCurrentLevel() * 0.1);
	}
}
