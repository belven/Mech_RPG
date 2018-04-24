// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MagazineWeapon.h"
#include "Sniper.generated.h"

/**
 *
 */
UCLASS()
class MECH_RPG_API USniper : public UMagazineWeapon
{
	GENERATED_BODY()


public:
	USniper();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
		static USniper* CreateSniper(AMech_RPGCharacter* inOwner);

	UFUNCTION(BlueprintCallable, Category = "Item")
		virtual UItem* Copy();

};
