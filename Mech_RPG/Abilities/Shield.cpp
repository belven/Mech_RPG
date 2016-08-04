// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "Abilities/Shield.h"
#include "Characters/Mech_RPGCharacter.h"

bool UShield::Activate(class AMech_RPGCharacter* target, FVector targetLocation)
{
	if (target != nullptr) {
		shieldHealth = GetWeaponHealthChange() * shieldAmount;
		this->target = target;
		SetOnCooldown(owner->GetWorld());
		target->OnPreHealthChange.AddUniqueDynamic(this, &UShield::ChangeHealth);
		return true;
	}
	return false;
}

UShield* UShield::CreateShield(float cooldown, AMech_RPGCharacter* owner, float inShieldAmount)
{
	UShield* ability = NewObject<UShield>(StaticClass());
	ability->SetCooldown(cooldown);
	ability->shieldAmount = inShieldAmount;
	ability->owner = owner;
	ability->AddTag(healTag, inShieldAmount);
	return ability;
}

FString UShield::GetTooltipText() 
{
	FString shieldString = FString::SanitizeFloat(GetWeaponHealthChange() *  shieldAmount);
	return "Shield" + UMiscLibrary::lnBreak + "Places a shield on target ally that Absorbs " + shieldString + "% of their max health as damage" + UMiscLibrary::lnBreak + "Cooldown: " + FString::SanitizeFloat(GetCooldown());
}

void UShield::ChangeHealth(FHealthChange healthChange) {
	if (healthChange.heals) {
		return;
	}

	if (healthChange.healthChange < shieldHealth) {
		shieldHealth -= healthChange.healthChange;
		healthChange.healthChange = 0;
	}
	else {
		healthChange.healthChange -= shieldHealth;
		target->OnPreHealthChange.RemoveDynamic(this, &UShield::ChangeHealth);
	}
}