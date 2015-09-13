// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "Mech_RPG.h"
#include "Weapon.h"
#include "Mech_RPGCharacter.h"

float AWeapon::GetDamage() {
	return damage;
}

float AWeapon::GetRange() {
	return range;
}

void AWeapon::SetDamage(float newVal) {
	damage = newVal;
}

void AWeapon::SetRange(float newVal) {
	range = newVal;
}

bool AWeapon::CanFire() {
	return canFire;
}

AWeapon* AWeapon::CreateWeapon(AActor* inOwner, float damage, float range, float fireRate, bool heals) {
	if (inOwner && inOwner->GetWorld()) {
		AWeapon* weapon = inOwner->GetWorld()->SpawnActor<AWeapon>(AWeapon::StaticClass());
		weapon->SetDamage(damage);
		weapon->SetRange(range);
		weapon->SetFireRate(fireRate);
		weapon->canFire = true;
		weapon->AttachRootComponentToActor(inOwner);
		weapon->lastTime = 0;
		weapon->SetHeals(heals);
		return weapon;
	}
	return NULL;
}

void AWeapon::Fire(AMech_RPGCharacter* target, AMech_RPGCharacter* owner) {
	FDamage damage;
	float damageDealt = GetDamage()  * owner->GetDamageModifier();

	damage.damager = owner;
	damage.target = target;
	damage.weaponUsed = this;

	damage.damagedDealt = heals ? -damageDealt : damageDealt;

	target->Hit(damage);
	canFire = false;
}

float AWeapon::GetFireRate() {
	return fireRate;
}

void AWeapon::SetFireRate(float newVal) {
	fireRate = newVal;
}

void AWeapon::Tick(float DeltaTime) {
	if (!canFire) {
		lastTime += DeltaTime;

		if (lastTime >= fireRate) {
			lastTime = 0;
			canFire = true;
		}
	}
}

bool AWeapon::Heals() {
	return heals;
}

void AWeapon::SetHeals(bool newVal) {
	heals = newVal;
}

AWeapon* AWeapon::CreatePresetWeapon(AMech_RPGCharacter* inOwner, TEnumAsByte<WeaponEnums::WeaponType> type) {
	switch (type) {
	case WeaponEnums::SMG:
		return CreateWeapon(inOwner, 30, 1000, 0.3);
		break;
	case WeaponEnums::Bio_Repair:
		return CreateWeapon(inOwner, 60, 600, 0.2, true);
		break;
	case WeaponEnums::RPG:
		return CreateWeapon(inOwner, 250, 1300, 2.5);
		break;
	case WeaponEnums::Shotgun:
		return CreateWeapon(inOwner, 70, 400, 0.8);
		break;
	case WeaponEnums::Sniper:
		return CreateWeapon(inOwner, 200, 1500, 2);
		break;
	}

	return  NULL;
}