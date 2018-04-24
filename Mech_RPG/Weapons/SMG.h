// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MagazineWeapon.h"
#include "SMG.generated.h"

/**
 * 
 */
UCLASS()
class MECH_RPG_API USMG : public UMagazineWeapon
{
	GENERATED_BODY()
	
	
public:
	USMG();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
		static USMG* CreateSMG(AMech_RPGCharacter* inOwner);

	UFUNCTION(BlueprintCallable, Category = "Item")
		virtual UItem* Copy();	
};
