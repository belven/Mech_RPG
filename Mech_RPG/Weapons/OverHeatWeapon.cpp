// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "Characters/Mech_RPGCharacter.h"
#include "OverHeatWeapon.h"

void AOverHeatWeapon::Tick(float DeltaTime)
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

float AOverHeatWeapon::GetProgressBarPercent()
{
	return heatLevel;
}

FLinearColor AOverHeatWeapon::GetProgressBarColour()
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

float AOverHeatWeapon::GetChangeAmount()
{
	return AWeapon::GetChangeAmount() * (1 + heatLevel);
}

void AOverHeatWeapon::SetItemOwner(AMech_RPGCharacter* inOwner)
{
	Super::SetItemOwner(inOwner);
	if (inOwner != nullptr)
	{
		inOwner->OnOutOfCombat.AddUniqueDynamic(this, &AOverHeatWeapon::Cooldown);
	}
}

void AOverHeatWeapon::Cooldown()
{
	heatLevel = 0;
	overHeated = false;
}

bool AOverHeatWeapon::CanFire()
{
	return !overHeated && Super::CanFire();
}

float AOverHeatWeapon::GetHeatLevel()
{
	return heatLevel;
}

// Only generate heat if we damage or heal
void AOverHeatWeapon::UseWeapon(AMech_RPGCharacter* target)
{
	bool isHealing = Heals() && UMiscLibrary::GetMissingHealth(target) > 0;
	bool isDamaging = !Heals() && target->GetHealth() > 0;

	if (isHealing || isDamaging)
	{
		heatLevel += heatGenerated * (1 + (heatLevel * 0.5));
	}

	Super::UseWeapon(target);
}

AOverHeatWeapon* AOverHeatWeapon::CreateOverHeatWeapon(UWorld* world, AMech_RPGCharacter* inOwner, FOverheatWeaponParams inSettings)
{
	if (world != nullptr)
	{
		AOverHeatWeapon* weapon = world->SpawnActor<AOverHeatWeapon>(AOverHeatWeapon::StaticClass());
		weapon->SetSettings(inSettings);
		weapon->heatLosePerTick = inSettings.heatLosePerTick;
		weapon->heatGenerated = inSettings.heatGenerated;
		weapon->SetItemOwner(inOwner);
		return weapon;
	}
	return NULL;
}