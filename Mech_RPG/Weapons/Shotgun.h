// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MagazineWeapon.h"
#include "Shotgun.generated.h"

/**
 *
 */
UCLASS()
class MECH_RPG_API UShotgun : public UMagazineWeapon
{
	GENERATED_BODY()
public:
	UShotgun();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
		static UShotgun* CreateShotgun(AMech_RPGCharacter* inOwner);

	UFUNCTION(BlueprintCallable, Category = "Item")
		virtual UItem* Copy();
};
