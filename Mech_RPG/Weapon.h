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
	bool canFire;
	float lastTime;
	bool heals;

public:
	float GetDamage();
	float GetRange();

	void SetDamage(float newVal);
	void SetRange(float newVal);

	bool CanFire();
	void Fire(class AMech_RPGCharacter* target, AMech_RPGCharacter* owner);

	static AWeapon* CreateWeapon(AActor* owner, float damage, float range, float fireRate, bool heals = false);

	float GetFireRate();
	void SetFireRate(float newVal);

	void Tick(float DeltaTime);
	bool Heals();
	void SetHeals(bool newVal);
};
