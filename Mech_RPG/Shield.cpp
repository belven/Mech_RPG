// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "Shield.h"
bool UShield::Activate(class AMech_RPGCharacter* target, FVector targetLocation)
{
	if (target != nullptr) {
		shieldHealth = shieldAmount >= 2 ? shieldAmount : target->GetMaxHealth() * shieldAmount;
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