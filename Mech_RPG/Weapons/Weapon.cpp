// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "Mech_RPG.h"
#include "Weapon.h"
#include "Characters/Mech_RPGCharacter.h"

UWeapon::UWeapon() : Super()
{
	partclSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("WeaponParticle"));
	audioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("WeaponAudio"));
	SetType(ItemEnumns::Weapon);

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleSystemClass(TEXT("/Game/TopDown/Particle_Effects/Gun_Flash"));
	if (ParticleSystemClass.Succeeded())
	{
		partclSystem->Template = ParticleSystemClass.Object;
		partclSystem->bAutoActivate = false;
		//partclSystem->SetActorParameter(FName(TEXT("BeamSource")), meshComponent);
	}
}

float UWeapon::GetChangeAmount()
{
	float tempDamage = settings.healthChange * (1 + (GetGrade() * 0.25));
	tempDamage *= (1 + (GetQuality() * 0.07));
	return tempDamage * GetItemOwner()->GetHealthChangeModifier();
}

float UWeapon::GetRange()
{
	return settings.range;
}

float UWeapon::GetDPS()
{
	return GetChangeAmount() * (1 / GetFireRate());
}

UItem* UWeapon::Copy()
{
	return CreateWeapon(GetItemOwner(), settings);
}

void UWeapon::SetChangeAmount(float newVal)
{
	settings.healthChange = newVal;
}

void UWeapon::SetRange(float newVal)
{
	settings.range = newVal;
}

bool UWeapon::CanFire()
{
	return canFire;
}

EDamageType UWeapon::GetChangeAmountType()
{
	return settings.damageType;
}

UWeapon* UWeapon::CreateWeapon(AMech_RPGCharacter* inOwner, FWeaponParams inSettings)
{
	UWeapon* weapon = NewObject<UWeapon>(StaticClass());
	weapon->SetSettings(inSettings);
	weapon->SetItemOwner(inOwner);
	return weapon;
}

void UWeapon::SetItemOwner(AMech_RPGCharacter* inOwner)
{
	UMechPart::SetItemOwner(inOwner);

	if (inOwner != nullptr)
	{
		if (partclSystem != nullptr)
		{
			// TODO Attach to end of weapon
			partclSystem->AttachToComponent(inOwner->GetRootComponent(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false));
			//partclSystem->SetActorParameter(FName(TEXT("BeamSource")), meshComponent);
		}

		inOwner->OnStopFiring.AddUniqueDynamic(this, &UWeapon::StopFire);
	}
}

float UWeapon::GetProgressBarPercent()
{
	return -1;
}

FLinearColor UWeapon::GetProgressBarColour()
{
	return FLinearColor::Blue;
}

FString UWeapon::GetTooltipText()
{
	FString dpsString = FString::FromInt(round(GetChangeAmount() * (1 / settings.fireRate)));

	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EDamageType"), true);

	FString damageType = !EnumPtr ? "Invalid" : EnumPtr->GetNameStringByIndex((int32)settings.damageType);

	return GetName() + UMiscLibrary::lnBreak + "DPS: " + dpsString + UMiscLibrary::lnBreak + "Damage Type: " + damageType;
}

void UWeapon::UseWeapon(AMech_RPGCharacter* target)
{
	FHealthChange healthChange;
	float changeAmount = GetChangeAmount();

	if (UMiscLibrary::IsCrit(settings.critChance))
	{
		changeAmount = changeAmount * 2;
		healthChange.crit = true;
	}

	if (partclSystem != nullptr)
	{
		if (!partclSystem->IsActive())
		{
			partclSystem->Activate(true);
			partclSystem->ActivateSystem(true);
		}
		partclSystem->SetActorParameter(FName(TEXT("BeamTarget")), target);
	}

	if (audioComp != nullptr && !audioComp->IsPlaying())
	{
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

	if (OnFire.IsBound())
	{
		OnFire.Broadcast();
	}
}

void UWeapon::StopFire()
{
	if (partclSystem != nullptr)
	{
		partclSystem->Deactivate();
	}

	if (audioComp != nullptr)
	{
		audioComp->Stop();
	}
}

float UWeapon::GetFireRate()
{
	float percentChange = 1 - (GetItemOwner()->GetAttackSpeedModifier() - 1);
	return settings.fireRate * percentChange;
}

void UWeapon::SetFireRate(float newVal)
{
	settings.fireRate = newVal;
}

float UWeapon::GetCritChance()
{
	return settings.critChance + GetItemOwner()->GetCritChanceModifier();
}

void UWeapon::SetCritChance(float newVal)
{
	settings.critChance = newVal;
}

void UWeapon::Tick(float DeltaTime)
{
	if (!canFire)
	{
		lastTime += DeltaTime;

		if (lastTime >= settings.fireRate)
		{
			lastTime = 0;
			canFire = true;
		}
	}
}

bool UWeapon::Heals()
{
	return settings.heals;
}

void UWeapon::SetHeals(bool newVal)
{
	settings.heals = newVal;
}

void UWeapon::SetSettings(FWeaponParams newSettings)
{
	settings = newSettings;
}

UWeapon* UWeapon::CreatePresetWeapon(AMech_RPGCharacter* inOwner,
	TEnumAsByte<WeaponEnums::WeaponType> weaponType /*= WeaponEnums::SMG*/, int32 weaponGrade /*= 0*/, int32 weaponQuality /*= 0*/)
{
	return UMiscLibrary::CreatePresetWeapon(inOwner, weaponType, weaponGrade, weaponQuality);
}

