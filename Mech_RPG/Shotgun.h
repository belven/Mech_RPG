// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MagazineWeapon.h"
#include "Shotgun.generated.h"

/**
 * 
 */
UCLASS()
class MECH_RPG_API AShotgun : public AMagazineWeapon
{
	GENERATED_BODY()
public:
		UFUNCTION(BlueprintCallable, Category = "Weapon")
		static AShotgun* CreateShotgun(AActor* inOwner);
	
	
	
};
