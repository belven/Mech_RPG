// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "Characters/Mech_RPGCharacter.h"
#include "MagazineWeapon.h"

void UMagazineWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (ammo <= 0)
	{
		Reload();
	}

	if (reloading)
	{
		lastTime += DeltaTime;

		if (lastTime >= 1)
		{
			lastTime = 0;

			ammo += reloadAmount;

			if (ammo >= magazineSize)
			{
				reloading = false;
				ammo = magazineSize;
			}
		}
	}
}

void UMagazineWeapon::Reload()
{
	reloading = true;
	StopFire();
}

void UMagazineWeapon::SetItemOwner(AMech_RPGCharacter* inOwner)
{
	Super::SetItemOwner(inOwner);
	if (inOwner != nullptr)
	{
		inOwner->OnOutOfCombat.AddUniqueDynamic(this, &UMagazineWeapon::FullReload);
	}
}

void UMagazineWeapon::FullReload()
{
	reloading = false;
	ammo = magazineSize;
}

bool UMagazineWeapon::CanFire()
{
	return !reloading && Super::CanFire();
}

float UMagazineWeapon::GetMagazineSize()
{
	return magazineSize;
}

float UMagazineWeapon::GetProgressBarPercent()
{
	return ammo / magazineSize;
}

FLinearColor UMagazineWeapon::GetProgressBarColour()
{
	if (reloading)
	{
		return FLinearColor::Red;
	}
	else
	{
		return FLinearColor::Green;
	}
}

float UMagazineWeapon::GetAmmo()
{
	return ammo;
}

void UMagazineWeapon::UseWeapon(AMech_RPGCharacter* target)
{
	Super::UseWeapon(target);
	ammo--;
}


UMagazineWeapon* UMagazineWeapon::CreateMagazineWeapon(AMech_RPGCharacter* inOwner, FMagazineWeaponParams inSettings)
{
	UMagazineWeapon* weapon = NewObject<UMagazineWeapon>(StaticClass());
	weapon->SetSettings(inSettings);
	weapon->magazineSize = inSettings.magazineSize;
	weapon->reloadAmount = inSettings.reloadAmount;
	weapon->ammo = inSettings.magazineSize;
	weapon->SetItemOwner(inOwner);
	return weapon;
}