// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "BindLife.h"
#include "Ability.h"
#include "Characters/Mech_RPGCharacter.h"

bool UBindLife::Activate(class AMech_RPGCharacter* target, FVector targetLocation)
{
	if (UMiscLibrary::IsCharacterAlive(target) && target != owner)
	{
		SetOnCooldown(owner->GetWorld());

		if (boundTarget != nullptr)
		{
			boundTarget->OnPreHealthChange.RemoveAll(this);
		}

		boundTarget = target;
		boundTarget->OnPreHealthChange.AddUniqueDynamic(this, &UBindLife::ChangeHealth);

		return true;
	}
	return false;
}


UBindLife* UBindLife::CreateBindLife(float cooldown, AMech_RPGCharacter* owner, float inMaxAmount, float inDamageReduction)
{
	UBindLife* ability = NewObject<UBindLife>(StaticClass());
	ability->SetCooldown(cooldown);
	ability->absorbPercent = inMaxAmount;
	ability->damageReduction = inDamageReduction;
	ability->owner = owner;
	ability->AddTag(healTag, 1);
	ability->affectedTeam = EAffectedTeam::Ally;
	return ability;
}

FString UBindLife::GetTooltipText()
{
	return "Bind Life" + UMiscLibrary::lnBreak + " transfers " + FString::SanitizeFloat(absorbPercent * 100)
		+ "% damage from nearby allies and reduces it by " + FString::SanitizeFloat(damageReduction * 100) + "%"
		+ UMiscLibrary::lnBreak + "Cooldown: " + FString::SanitizeFloat(GetCooldown());
}

void UBindLife::ChangeHealth(FHealthChange& healthChange)
{
	if (!mIsChildOf(healthChange.abilityUsed, UBindLife::StaticClass()))
	{
		//UE_LOG(AbilitiesLog, Log, TEXT("Life absorbed %d"), healthChange.changeAmount * (1 - absorbPercent));

		healthChange.changeAmount *= (1 - absorbPercent);

		FHealthChange change;
		change.heals = false;
		change.changeAmount = healthChange.changeAmount * (1 - damageReduction);
		change.manipulator = healthChange.target;
		change.target = owner;
		change.abilityUsed = this;
		owner->ChangeHealth(change);
	}
}
