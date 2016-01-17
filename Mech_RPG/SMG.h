// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MagazineWeapon.h"
#include "SMG.generated.h"

/**
 * 
 */
UCLASS()
class MECH_RPG_API ASMG : public AMagazineWeapon
{
	GENERATED_BODY()
	
	
public:
	UFUNCTION(BlueprintCallable, Category = "Weapon")
		static ASMG* CreateSMG(AMech_RPGCharacter* inOwner);

	
};
