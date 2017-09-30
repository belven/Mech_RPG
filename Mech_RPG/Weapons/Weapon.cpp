// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "Mech_RPG.h"
#include "Weapon.h"
#include "Characters/Mech_RPGCharacter.h"

AWeapon::AWeapon() : Super() {
	partclSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("WeaponParticle"));
	audioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("WeaponAudio"));
	SetType(ItemEnumns::Weapon);

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
	return tempDamage * GetItemOwner()->GetHealthChangeModifier();
}

float AWeapon::GetRange() {
	return settings.range;
}

float AWeapon::GetDPS()
{
	return GetChangeAmount() * (1 / GetFireRate());
}

AItem* AWeapon::Copy()
{
	return CreateWeapon(GetWorld(), GetItemOwner(), settings);
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

EDamageType AWeapon::GetChangeAmountType() {
	return settings.damageType;
}

AWeapon* AWeapon::CreateWeapon(UWorld* world, AMech_RPGCharacter* inOwner, FWeaponParams inSettings) {
	if (world != nullptr) {
		AWeapon* weapon = world->SpawnActor<AWeapon>(AWeapon::StaticClass());
		weapon->SetSettings(inSettings);
		weapon->SetItemOwner(inOwner);
		return weapon;
	}
	return nullptr;
}

void AWeapon::SetItemOwner(AMech_RPGCharacter* inOwner) {
	Super::SetItemOwner(inOwner);

	if (inOwner != nullptr) {
		if (partclSystem != nullptr) {
			// TODO Attach to end of weapon
			partclSystem->AttachToComponent(inOwner->GetRootComponent(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false));
			partclSystem->SetActorParameter(FName(TEXT("BeamSource")), this);
		}

		inOwner->OnStopFiring.AddUniqueDynamic(this, &AWeapon::StopFire);
	}
}

float AWeapon::GetProgressBarPercent() {
	return -1;
}

FString AWeapon::GetTooltipText()
{
	FString dpsString = FString::FromInt(round(GetChangeAmount() * (1 / settings.fireRate)));

	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EDamageType"), true);

	FString damageType = !EnumPtr ? "Invalid" : EnumPtr->GetNameStringByIndex((int32)settings.damageType);

	return GetName() + UMiscLibrary::lnBreak + "DPS: " + dpsString + UMiscLibrary::lnBreak + "Damage Type: " + damageType;
}

void AWeapon::Fire(AMech_RPGCharacter* target) {
	FHealthChange healthChange;
	float changeAmount = GetChangeAmount();

	if (UMiscLibrary::IsCrit(settings.critChance)) {
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

	healthChange.manipulator = GetItemOwner();
	healthChange.target = target;
	healthChange.weaponUsed = this;
	healthChange.damageType = settings.damageType;
	healthChange.changeAmount = changeAmount;
	healthChange.heals = settings.heals;

	target->ChangeHealth(healthChange);
	canFire = false;

	if (OnFire.IsBound()) {
		OnFire.Broadcast();
	}
}

//void AWeapon::Fire(ACover* target) {
//	FHealthChange healthChange;
//	float changeAmount = GetChangeAmount()  * GetItemOwner()->GetHealthChangeModifier();
//
//	healthChange.manipulator = GetItemOwner();
//	//healthChange.target = target;
//	healthChange.weaponUsed = this;
//
//	healthChange.healthChange = settings.heals ? -changeAmount : changeAmount;
//
//	target->ChangeHealth(healthChange);
//	canFire = false;
//}

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
	Super::Tick(DeltaTime);
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

AWeapon* AWeapon::CreatePresetWeapon(UWorld* world, AMech_RPGCharacter* inOwner, 
	TEnumAsByte<WeaponEnums::WeaponType> weaponType, int32 weaponGrade, int32 weaponQuality) {
	return UMiscLibrary::CreatePresetWeapon(world, inOwner, weaponType, weaponGrade, weaponQuality);
}

