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
	ASniper();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
		static ASniper* CreateSniper(UWorld* world, AMech_RPGCharacter* inOwner);

	UFUNCTION(BlueprintCallable, Category = "Item")
		virtual AItem* Copy();

};
