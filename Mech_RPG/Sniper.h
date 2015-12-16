// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MagazineWeapon.h"
#include "Sniper.generated.h"

/**
 *
 */
UCLASS()
class MECH_RPG_API ASniper : public AMagazineWeapon
{
	GENERATED_BODY()


public:
	UFUNCTION(BlueprintCallable, Category = "Weapon")
		static ASniper* CreateSniper(AActor* inOwner);

};
