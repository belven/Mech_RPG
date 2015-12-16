// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "Mech_RPG.h"
#include "Weapon.h"
#include "OverHeatWeapon.h"
#include "MagazineWeapon.h"
#include "SMG.h"
#include "Shotgun.h"
#include "Sniper.h"

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

DamageEnums::DamageType AWeapon::GetDamageType() {
	return settings.damageType;
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
	bool isCrit = settings.critChance <= rand() % 100;

	if (isCrit) {
		damageDealt = damageDealt * 2;
	}


	damage.damager = owner;
	damage.target = target;
	damage.weaponUsed = this;
	damage.damageType = settings.damageType;
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
	FMagazineWeaponParams magSettings;

	switch (type) {
	case WeaponEnums::SMG:
		return ASMG::CreateSMG(inOwner);
	case WeaponEnums::Bio_Repair:
		magSettings.damage = 45;
		magSettings.range = 600;
		magSettings.fireRate = 0.2;
		magSettings.heals = true;
		return AOverHeatWeapon::CreateOverHeatWeapon(inOwner, magSettings);
	case WeaponEnums::RPG:
		magSettings.damage = 500;
		magSettings.range = 1300;
		magSettings.fireRate = 2.5;
		magSettings.heals = false;
		return CreateWeapon(inOwner, magSettings);
	case WeaponEnums::Shotgun:
		return AShotgun::CreateShotgun(inOwner);
	case WeaponEnums::Sniper:
		return ASniper::CreateSniper(inOwner);
	}

	return  NULL;
}