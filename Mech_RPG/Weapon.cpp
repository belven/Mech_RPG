// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "Mech_RPG.h"
#include "Weapon.h"
#include "OverHeatWeapon.h"
#include "MagazineWeapon.h"

float AWeapon::GetDamage() {
	return settings.damage;
}

float AWeapon::GetRange() {
	return settings.range;
}

void AWeapon::SetDamage(float newVal) {
	settings.damage = newVal;
}

void AWeapon::SetRange(float newVal) {
	settings.range = newVal;
}

bool AWeapon::CanFire() {
	return canFire;
}

AWeapon* AWeapon::CreateWeapon(AActor* inOwner, FWeaponParams inSettings) {
	if (inOwner && inOwner->GetWorld()) {
		AWeapon* weapon = inOwner->GetWorld()->SpawnActor<AWeapon>(AWeapon::StaticClass());
		weapon->settings = inSettings;
		weapon->canFire = true;
		weapon->AttachRootComponentToActor(inOwner);
		weapon->lastTime = 0;
		return weapon;
	}
	return NULL;
}

float AWeapon::GetProgressBarPercent() {
	return -1;
}

void AWeapon::Fire(AMech_RPGCharacter* target, AMech_RPGCharacter* owner) {
	FHealthChange damage;
	float damageDealt = GetDamage()  * owner->GetDamageModifier();

	damage.damager = owner;
	damage.target = target;
	damage.weaponUsed = this;

	damage.healthChange = settings.heals ? -damageDealt : damageDealt;

	target->ChangeHealth(damage);
	canFire = false;
}

void AWeapon::Fire(ACover* target, AMech_RPGCharacter* owner) {
	FHealthChange damage;
	float damageDealt = GetDamage()  * owner->GetDamageModifier();

	damage.damager = owner;
	//damage.target = target;
	damage.weaponUsed = this;

	damage.healthChange = settings.heals ? -damageDealt : damageDealt;

	target->ChangeHealth(damage);
	canFire = false;
}

float AWeapon::GetFireRate() {
	return settings.fireRate;
}

void AWeapon::SetFireRate(float newVal) {
	settings.fireRate = newVal;
}

void AWeapon::Tick(float DeltaTime) {
	if (!canFire) {
		lastTime += DeltaTime;

		if (lastTime >= settings.fireRate) {
			lastTime = 0;
			canFire = true;
		}
	}
}

bool AWeapon::Heals() {
	return settings.heals;
}

void AWeapon::SetHeals(bool newVal) {
	settings.heals = newVal;
}

AWeapon* AWeapon::CreatePresetWeapon(AMech_RPGCharacter* inOwner, TEnumAsByte<WeaponEnums::WeaponType> type) {
	FWeaponParams settings;
	FMagazineWeaponParams magSettings;

	switch (type) {
	case WeaponEnums::SMG:
		magSettings.damage = 35;
		magSettings.range = 1000;
		magSettings.fireRate = 0.3;
		magSettings.heals = false;
		magSettings.magazineSize = 10;
		magSettings.reloadAmount = 1;
		return AMagazineWeapon::CreateMagazineWeapon(inOwner, magSettings);
	case WeaponEnums::Bio_Repair:
		settings.damage = 45;
		settings.range = 600;
		settings.fireRate = 0.2;
		settings.heals = true;
		return AOverHeatWeapon::CreateOverHeatWeapon(inOwner, settings);
	case WeaponEnums::RPG:
		settings.damage = 500;
		settings.range = 1300;
		settings.fireRate = 2.5;
		settings.heals = false;
		return CreateWeapon(inOwner, settings);
	case WeaponEnums::Shotgun:
		settings.damage = 150;
		settings.range = 400;
		settings.fireRate = 0.8;
		settings.heals = false;
		return CreateWeapon(inOwner, settings);
	case WeaponEnums::Sniper:
		settings.damage = 250;
		settings.range = 1500;
		settings.fireRate = 2;
		settings.heals = false;
		return AOverHeatWeapon::CreateOverHeatWeapon(inOwner, settings);
	}

	return  NULL;
}