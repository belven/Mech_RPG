// Copyright of Explosive Industries

#pragma once

#include "OverHeatWeapon.h"
#include "Bio_Rifle.generated.h"

/**
 *
 */
UCLASS()
class MECH_RPG_API UBio_Rifle : public UOverHeatWeapon
{
	GENERATED_BODY()

public:
	UBio_Rifle();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
		static UBio_Rifle* CreateBioRifle(AMech_RPGCharacter* inOwner);

	UFUNCTION(BlueprintCallable, Category = "Item")
		UItem* Copy() override;

};
