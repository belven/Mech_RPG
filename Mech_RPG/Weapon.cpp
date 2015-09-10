// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "Mech_RPG.h"
#include "Weapon.h"
#include "Mech_RPGCharacter.h"

float AWeapon::GetDamage(){
	return damage;
}

float AWeapon::GetRange(){
	return range;
}

void AWeapon::SetDamage(float newVal){
	damage = newVal;
}

void AWeapon::SetRange(float newVal){
	range = newVal;
}

bool AWeapon::CanFire() {
	return canFire;
}

AWeapon* AWeapon::CreateWeapon(AActor* owner, float damage, float range, float fireRate, bool heals){
	if (owner && owner->GetWorld()){
		AWeapon* weapon = owner->GetWorld()->SpawnActor<AWeapon>(AWeapon::StaticClass());
		weapon->SetDamage(damage);
		weapon->SetRange(range);
		weapon->SetFireRate(fireRate);
		weapon->canFire = true;
		weapon->AttachRootComponentToActor(owner);
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

float AWeapon::GetFireRate(){
	return fireRate;
}

void AWeapon::SetFireRate(float newVal){
	fireRate = newVal;
}

void AWeapon::Tick(float DeltaTime){
	if (!canFire) {
		lastTime += DeltaTime;

		if (lastTime >= fireRate){
			lastTime = 0;
			canFire = true;
		}
	}
}

bool AWeapon::Heals(){
	return heals;
}

void AWeapon::SetHeals(bool newVal){
	heals = newVal;
}

AWeapon* AWeapon::CreatePresetWeapon(AMech_RPGCharacter* owner, TEnumAsByte<WeaponEnums::WeaponType> type) {
	switch (type) {
	case WeaponEnums::SMG:
		return CreateWeapon(owner, 30, 1000, 0.3);
		break;
	case WeaponEnums::Bio_Repair:
		return CreateWeapon(owner, 400, 1000, 2, true);
		break;
	case WeaponEnums::RPG:
		return CreateWeapon(owner, 250, 1300, 2.5);
		break;
	case WeaponEnums::Shotgun:
		return CreateWeapon(owner, 70, 350, 0.8);
		break;
	case WeaponEnums::Sniper:
		return CreateWeapon(owner, 250, 1500, 2);
		break;
	}

	return  NULL;
}