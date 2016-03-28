// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "Mech_RPG.h"
#include "Weapon.h"
#include "OverHeatWeapon.h"
#include "MagazineWeapon.h"
#include "SMG.h"
#include "Shotgun.h"
#include "Sniper.h"
#include "Bio_Rifle.h"

AWeapon::AWeapon() : Super() {
	partclSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("WeaponParticle"));
	audioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("WeaponAudio"));

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleSystemClass(TEXT("/Game/TopDown/Particle_Effects/Gun_Flash"));
	if (ParticleSystemClass.Succeeded()) {
		partclSystem->Template = ParticleSystemClass.Object;
		partclSystem->bAutoActivate = false;
		partclSystem->SetActorParameter(FName(TEXT("BeamSource")), this);
	}
}

float AWeapon::GetChangeAmount() {
	float tempDamage = settings.healthChange * (1 + (GetGrade() * 0.25));
	tempDamage *= (1 + (GetQuality() * 0.07));
	return tempDamage;
}

float AWeapon::GetRange() {
	return settings.range;
}

void AWeapon::SetChangeAmount(float newVal) {
	settings.healthChange = newVal;
}

void AWeapon::SetRange(float newVal) {
	settings.range = newVal;
}

bool AWeapon::CanFire() {
	return canFire;
}

DamageEnums::DamageType AWeapon::GetChangeAmountType() {
	return settings.damageType;
}

AWeapon* AWeapon::CreateWeapon(AMech_RPGCharacter* inOwner, FWeaponParams inSettings) {
	if (inOwner && inOwner->GetWorld()) {
		AWeapon* weapon = inOwner->GetWorld()->SpawnActor<AWeapon>(AWeapon::StaticClass());
		weapon->SetSettings(inSettings);
		weapon->SetOwner(inOwner);
		return weapon;
	}
	return nullptr;
}

void AWeapon::SetOwner(AMech_RPGCharacter* inOwner) {
	Super::SetOwner(inOwner);

	if (partclSystem != nullptr) {
		// TODO Attach to end of weapon
		partclSystem->AttachTo(inOwner->GetRootComponent());
	}

	inOwner->OnStopFiring.AddDynamic(this, &AWeapon::StopFire);
}

float AWeapon::GetProgressBarPercent() {
	return -1;
}

void AWeapon::Fire(AMech_RPGCharacter* target) {
	FHealthChange healthChange;
	float changeAmount = GetChangeAmount() * GetOwner()->GetHealthChangeModifier();
	bool isCrit = rand() % 100 <= settings.critChance;

	if (isCrit) {
		changeAmount = changeAmount * 2;
		healthChange.crit = true;
	}

	if (partclSystem != nullptr) {
		if (!partclSystem->IsActive()) {
			partclSystem->Activate(true);
			partclSystem->ActivateSystem(true);
		}
		partclSystem->SetActorParameter(FName(TEXT("BeamTarget")), target);
	}

	if (audioComp != nullptr && !audioComp->IsPlaying()) {
		audioComp->Play(0);
	}

	healthChange.damager = GetOwner();
	healthChange.target = target;
	healthChange.weaponUsed = this;
	healthChange.damageType = settings.damageType;
	healthChange.healthChange = changeAmount;
	healthChange.heals = settings.heals;

	target->ChangeHealth(healthChange);
	canFire = false;

	if (OnFire.IsBound()) {
		OnFire.Broadcast();
	}
}

void AWeapon::Fire(ACover* target) {
	FHealthChange healthChange;
	float changeAmount = GetChangeAmount()  * GetOwner()->GetHealthChangeModifier();

	healthChange.damager = GetOwner();
	//healthChange.target = target;
	healthChange.weaponUsed = this;

	healthChange.healthChange = settings.heals ? -changeAmount : changeAmount;

	target->ChangeHealth(healthChange);
	canFire = false;
}

void AWeapon::StopFire()
{
	if (partclSystem != nullptr) {
		partclSystem->Deactivate();
	}

	if (audioComp != nullptr) {
		audioComp->Stop();
	}
}

float AWeapon::GetFireRate() {
	return settings.fireRate;
}

void AWeapon::SetFireRate(float newVal) {
	settings.fireRate = newVal;
}

float AWeapon::GetCritChance() {
	return settings.critChance;
}

void AWeapon::SetCritChance(float newVal) {
	settings.critChance = newVal;
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

void AWeapon::SetSettings(FWeaponParams newSettings)
{
	settings = newSettings;
}

AWeapon* AWeapon::CreatePresetWeapon(AMech_RPGCharacter* inOwner, TEnumAsByte<WeaponEnums::WeaponType> type, int32 grade, int32 quality) {
	FMagazineWeaponParams magSettings;
	AWeapon* weapon = nullptr;

	switch (type) {
	case WeaponEnums::SMG:
		weapon = ASMG::CreateSMG(inOwner);
		break;
	case WeaponEnums::Bio_Repair:
		weapon = ABio_Rifle::CreateBioRifle(inOwner);
		break;
	case WeaponEnums::RPG:
		magSettings.healthChange = 500;
		magSettings.range = 1300;
		magSettings.fireRate = 2.5;
		magSettings.heals = false;
		weapon = CreateWeapon(inOwner, magSettings);
		break;
	case WeaponEnums::Shotgun:
		weapon = AShotgun::CreateShotgun(inOwner);
		break;
	case WeaponEnums::Sniper:
		weapon = ASniper::CreateSniper(inOwner);
		break;
	}

	weapon->SetGrade(grade);
	weapon->SetQuality(quality);

	return weapon;
}