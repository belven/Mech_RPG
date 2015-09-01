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


AWeapon* AWeapon::CreateWeapon(AActor* owner, float damage, float range, float fireRate){
	if (owner && owner->GetWorld()){
		AWeapon* weapon = owner->GetWorld()->SpawnActor<AWeapon>(AWeapon::StaticClass());
		weapon->SetDamage(damage);
		weapon->SetRange(range);
		weapon->SetFireRate(fireRate);
		weapon->canFire = true;
		weapon->AttachRootComponentToActor(owner);
		weapon->lastTime = 0;
		return weapon;
	}
	return NULL;
}

void AWeapon::Fire(AMech_RPGCharacter* target, AMech_RPGCharacter* owner) {
	target->Hit(owner, GetDamage());
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