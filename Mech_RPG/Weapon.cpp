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