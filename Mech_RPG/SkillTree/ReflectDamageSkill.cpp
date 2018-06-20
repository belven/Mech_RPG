// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "ReflectDamageSkill.h"




float UReflectDamageSkill::GetStatBonus(EStatEnum statType)
{
	return 0.0;
}

UReflectDamageSkill* UReflectDamageSkill::CreateReflectDamageSkill()
{
	UReflectDamageSkill* skill = NewObject<UReflectDamageSkill>(StaticClass());
	skill->SetName("Reflective Shielding");
	skill->SetStartLevel(0);
	skill->SetMaxLevel(20);
	skill->SetCurrentLevel(0);
	return skill;
}

FString UReflectDamageSkill::GetTooltipText()
{
	FString tooltip = "Has a ";
	tooltip += FString::SanitizeFloat(GetCurrentLevel() * 0.05 * 100);
	tooltip += "% ";
	tooltip += "chance to reflect ";
	tooltip += FString::SanitizeFloat(GetCurrentLevel() * 30);
	tooltip += " damage back to the attacker";

	return tooltip;
}

void UReflectDamageSkill::PreHealthChanged(FHealthChange& healthChange)
{
	if (!healthChange.heals && UMiscLibrary::IsSuccess(GetCurrentLevel() * 0.05 * 100))
	{
		// Reflect Damage
		FHealthChange reflection;
		reflection.heals = false;
		reflection.manipulator = healthChange.target;
		reflection.target = healthChange.manipulator;
		reflection.changeAmount = GetCurrentLevel() * 30;
		reflection.damageType = healthChange.damageType;
		reflection.skillNode = this;
		healthChange.manipulator->ChangeHealth(reflection);
	}
}
