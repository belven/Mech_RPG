// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "Characters/Mech_RPGCharacter.h"
#include "OverHeatWeapon.h"

void UOverHeatWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (heatLevel >= 1)
	{
		overHeated = true;
		heatLevel = 1;
	}

	if (heatLevel > 0)
	{
		if (!overHeated)
		{
			// It takes 20 seconds to reduce 100 heat
			heatLevel -= heatLosePerTick * DeltaTime;
		}
		else
		{
			StopFire();
			// If takes 6.45 ish seconds to loose 100 heat when overHeated is TRUE
			heatLevel -= 0.12 * DeltaTime;

			if (heatLevel < 0)
			{
				heatLevel = 0;
				overHeated = false;
			}
		}
	}

	if (heatLevel < 0)
	{
		heatLevel = 0;
	}
}

float UOverHeatWeapon::GetProgressBarPercent()
{
	return heatLevel;
}

FLinearColor UOverHeatWeapon::GetProgressBarColour()
{
	if (overHeated)
	{
		return FLinearColor::Red;
	}
	else if (heatLevel > 0.75)
	{
		return FLinearColor::FromSRGBColor(FColor::Orange);
	}
	else if (heatLevel > 0.49)
	{
		return FLinearColor::Yellow;
	}
	else
	{
		return FLinearColor::Green;
	}
}

float UOverHeatWeapon::GetChangeAmount()
{
	return UWeapon::GetChangeAmount() * (1 + heatLevel);
}

void UOverHeatWeapon::SetItemOwner(AMech_RPGCharacter* inOwner)
{
	Super::SetItemOwner(inOwner);
	if (inOwner != nullptr)
	{
		inOwner->OnOutOfCombat.AddUniqueDynamic(this, &UOverHeatWeapon::Cooldown);
	}
}

void UOverHeatWeapon::Cooldown()
{
	heatLevel = 0;
	overHeated = false;
}

bool UOverHeatWeapon::CanFire()
{
	return !overHeated && Super::CanFire();
}

float UOverHeatWeapon::GetHeatLevel()
{
	return heatLevel;
}

// Only generate heat if we damage or heal
void UOverHeatWeapon::UseWeapon(AMech_RPGCharacter* target)
{
	bool isHealing = Heals() && UMiscLibrary::GetMissingHealth(target) > 0;
	bool isDamaging = !Heals() && target->GetHealth() > 0;

	if (isHealing || isDamaging)
	{
		heatLevel += heatGenerated * (1 + (heatLevel * 0.5));
	}

	Super::UseWeapon(target);
}

UOverHeatWeapon* UOverHeatWeapon::CreateOverHeatWeapon(AMech_RPGCharacter* inOwner, FOverheatWeaponParams inSettings)
{
	UOverHeatWeapon* weapon = NewObject<UOverHeatWeapon>(StaticClass());
	weapon->SetSettings(inSettings);
	weapon->heatLosePerTick = inSettings.heatLosePerTick;
	weapon->heatGenerated = inSettings.heatGenerated;
	weapon->SetItemOwner(inOwner);
	return weapon;
}