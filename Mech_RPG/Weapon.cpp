// Fill out your copyright notice in the Description page of Project Settings.

#include "Mech_RPG.h"
#include "Weapon.h"

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

bool AWeapon::CanFire(float deltaTime) {
	static float lastTime = 0;
	lastTime += deltaTime;

	if (lastTime >= fireRate){
		lastTime = 0;
		return true;
	}
	return false;
}


AWeapon* AWeapon::CreateWeapon(AActor* owner, float damage, float range, float fireRate){
	if (owner && owner->GetWorld()){
		AWeapon* weapon = owner->GetWorld()->SpawnActor<AWeapon>(AWeapon::StaticClass());
		weapon->SetDamage(damage);
		weapon->SetRange(range);
		weapon->SetFireRate(fireRate);
		return weapon;
	}
	return NULL;
}


float AWeapon::GetFireRate(){
	return fireRate;
}


void AWeapon::SetFireRate(float newVal){
	fireRate = newVal;
}