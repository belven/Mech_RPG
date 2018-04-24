// Copyright of Explosive Industries

#pragma once

#include "OverHeatWeapon.h"
#include "LaserSniper.generated.h"

/**
 * 
 */
UCLASS()
class MECH_RPG_API ULaserSniper : public UOverHeatWeapon
{
	GENERATED_BODY()
public:
	ULaserSniper();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
		static ULaserSniper* CreateLaserSniper(AMech_RPGCharacter* inOwner);

	UFUNCTION(BlueprintCallable, Category = "Item")
		virtual UItem* Copy();
	
	
	
	
};
