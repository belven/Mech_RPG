// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "ShieldBomb.h"
#include "Characters/Mech_RPGCharacter.h"
#include <iostream>
#include <Kismet/KismetMathLibrary.h>


bool UShieldBomb::Activate(class AMech_RPGCharacter* target, FVector targetLocation)
{
	if (UMiscLibrary::IsCharacterAlive(target) && !target->OnPreHealthChange.Contains(this, "ChangeHealth"))
	{
		shieldHealth = shieldAmount;
		shieldTarget = target;
		SetOnCooldown(owner->GetWorld());
		shieldTarget->OnPreHealthChange.AddUniqueDynamic(this, &UShieldBomb::ChangeHealth);
		UE_LOG(AbilitiesLog, Log, TEXT("%d used %s on %d"), owner->GetID(), *GetClass()->GetName(), target->GetID());
		return true;
	}
	return false;
}

UShieldBomb* UShieldBomb::CreateShieldBomb(float cooldown, AMech_RPGCharacter* owner, float inShieldAmount)
{
	UShieldBomb* ability = NewObject<UShieldBomb>(StaticClass());
	ability->SetCooldown(cooldown);
	ability->shieldAmount = inShieldAmount;
	ability->owner = owner;
	ability->AddTag(healTag, 1);
	ability->AddTag(selfTargetted, 1);
	ability->affectedTeam = EAffectedTeam::Ally;
	return ability;
}

FString UShieldBomb::GetTooltipText()
{
	FString shieldString = FString::SanitizeFloat(shieldAmount);
	return "Shield" + UMiscLibrary::lnBreak + "Places a shield on target ally that Absorbs " + shieldString + " damage" + UMiscLibrary::lnBreak + "Cooldown: " + FString::SanitizeFloat(GetCooldown());
}

void UShieldBomb::ChangeHealth(FHealthChange& healthChange)
{
	if (healthChange.heals || !UMiscLibrary::IsCharacterAlive(healthChange.manipulator))
	{
		return;
	}

	FVector enemyLocation = healthChange.manipulator->GetActorLocation();
	float ownerRot = owner->GetActorRotation().Clamp().Yaw;
	float rot = UKismetMathLibrary::FindLookAtRotation(owner->GetActorLocation(), enemyLocation).Clamp().Yaw;

	float angle = rot - ownerRot;

	if (angle < 0)
		angle += 360;
	UE_LOG(AbilitiesLog, Log, TEXT("Angle %f"), angle);

	if ((shieldHealth - healthChange.changeAmount) > 0)
	{
		if (angle <= 20 || angle >= 340)
		{
			shieldHealth -= healthChange.changeAmount;

			FHealthChange reflection;
			reflection.heals = false;
			reflection.manipulator = owner;
			reflection.target = healthChange.manipulator;
			reflection.weaponUsed = nullptr;
			reflection.changeAmount = healthChange.changeAmount * 0.3;
			reflection.damageType = EDamageType::Energy;
			reflection.ignoresArmour = true;

			healthChange.manipulator->ChangeHealth(reflection);

			healthChange.changeAmount *= 0.7;
		}
	}
	else
	{
		shieldHealth -= healthChange.changeAmount;

		for (AMech_RPGCharacter* character : UMiscLibrary::GetCharactersInRange(1500.0F, owner))
		{
			enemyLocation = character->GetActorLocation();

			//angle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(enemyLocation, ownerForwards)));

			if (character != owner)
			{
				FHealthChange explosion;
				explosion.heals = false;
				explosion.manipulator = owner;
				explosion.target = character;
				explosion.changeAmount = shieldAmount;
				explosion.damageType = EDamageType::Blast;
				character->ChangeHealth(explosion);
			}
		}

		shieldTarget->OnPreHealthChange.RemoveDynamic(this, &UShieldBomb::ChangeHealth);
	}
}

