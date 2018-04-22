// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "Abilities/Shield.h"
#include "Characters/Mech_RPGCharacter.h"

bool UShield::Activate(class AMech_RPGCharacter* target, FVector targetLocation)
{
	if (UMiscLibrary::IsCharacterAlive(target))
	{
		shieldHealth = GetWeaponHealthChange() * shieldAmount;
		shieldTarget = target;
		SetOnCooldown(owner->GetWorld());
		shieldTarget->OnPreHealthChange.AddUniqueDynamic(this, &UShield::ChangeHealth);
		UE_LOG(AbilitiesLog, Log, TEXT("%d used %s on %d"), owner->GetID(), *GetClass()->GetName(), target->GetID());
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
	ability->affectedTeam = EAffectedTeam::Ally;
	return ability;
}

FString UShield::GetTooltipText()
{
	FString shieldString = FString::SanitizeFloat(GetWeaponHealthChange() *  shieldAmount);
	return "Shield" + UMiscLibrary::lnBreak + "Places a shield on target ally that Absorbs " + shieldString + " damage" + UMiscLibrary::lnBreak + "Cooldown: " + FString::SanitizeFloat(GetCooldown());
}

void UShield::ChangeHealth(FHealthChange& healthChange)
{
	if (healthChange.heals || !UMiscLibrary::IsCharacterAlive(shieldTarget))
	{
		return;
	}

	if (healthChange.changeAmount < shieldHealth)
	{
		shieldHealth -= healthChange.changeAmount;
		healthChange.changeAmount = 0;
	}
	else
	{
		healthChange.changeAmount -= shieldHealth;
		shieldTarget->OnPreHealthChange.RemoveDynamic(this, &UShield::ChangeHealth);
	}
}