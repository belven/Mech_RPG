// Copyright of Explosive Industries

#pragma once

#include "OverHeatWeapon.h"
#include "LaserSniper.generated.h"

/**
 * 
 */
UCLASS()
class MECH_RPG_API ALaserSniper : public AOverHeatWeapon
{
	GENERATED_BODY()
public:
	ALaserSniper();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
		static ALaserSniper* CreateLaserSniper(UWorld* world, AMech_RPGCharacter* inOwner);

	UFUNCTION(BlueprintCallable, Category = "Item")
		virtual AItem* Copy();
	
	
	
	
};
