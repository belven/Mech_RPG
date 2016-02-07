// Copyright of Explosive Industries

#pragma once

#include "OverHeatWeapon.h"
#include "Bio_Rifle.generated.h"

/**
 *
 */
UCLASS()
class MECH_RPG_API ABio_Rifle : public AOverHeatWeapon
{
	GENERATED_BODY()

public:
	ABio_Rifle();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
		static ABio_Rifle* CreateBioRifle(AMech_RPGCharacter* inOwner);

};
