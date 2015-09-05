// Fill out your copyright notice in the Description page of Project Settings.

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
	target->Hit(owner, heals ? -GetDamage() : GetDamage());
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
		return CreateWeapon(owner, 20, 600, 0.3);
		break;
	case WeaponEnums::Bio_Repair:
		return CreateWeapon(owner, 300, 1000, 2, true);
		break;
	case WeaponEnums::RPG:
		return CreateWeapon(owner, 500, 1300, 2.5);
		break;
	case WeaponEnums::Shotgun:
		return CreateWeapon(owner, 100, 300, 0.8);
		break;
	case WeaponEnums::Sniper:
		return CreateWeapon(owner, 250, 2500, 2);
		break;
	}

	return  NULL;
}