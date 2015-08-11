// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MechAttachment.h"
#include "Weapon.generated.h"

/**
 * 
 */
UCLASS()
class MECH_RPG_API AWeapon : public AMechAttachment
{
	GENERATED_BODY()
private:
	float damage;
	float range;	
	float fireRate;

public:
	float GetDamage();
	float GetRange();

	void SetDamage(float newVal);
	void SetRange(float newVal);

	bool CanFire(float deltaTime);

	static AWeapon* CreateWeapon(AActor* owner, float damage, float range, float fireRate);
	float GetFireRate();
	void SetFireRate(float newVal);
};
