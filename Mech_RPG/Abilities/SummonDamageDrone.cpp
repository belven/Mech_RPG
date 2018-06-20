// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "AllyAIController.h"
#include "Characters/Drone.h"
#include "SummonDamageDrone.h"

bool USummonDamageDrone::Activate(class AMech_RPGCharacter* target, FVector targetLocation)
{
	if (droneSummoned == nullptr)
	{
		FNavLocation nav;
		int count = 0;

		while (droneSummoned == nullptr && count < 10)
		{
			owner->GetWorld()->GetNavigationSystem()->GetRandomPointInNavigableRadius(owner->GetActorLocation(), 400, nav);
			FVector loc;
			loc = nav.Location;
			//loc.Z += (droneSummoned->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight());
			droneSummoned = UMiscLibrary::SpawnCharacter<ADrone>(owner->GetWorld(), loc, owner->GetActorRotation(), ADrone::StaticClass());
			count++;
		}

		if (droneSummoned != nullptr)
		{
			droneSummoned->SetDroneOwner(owner);
			droneSummoned->SetGroup(owner->GetGroup());
			droneSummoned->CreatePresetRole(ERole::DPS);
			droneSummoned->SetTeam(owner->GetTeam());
			droneSummoned->SetDead(false);
			SetOnCooldown(owner->GetWorld());
			return true;
		}
	}
	else
	{
		droneSummoned->Resurrect();
	}
	return false;
}


USummonDamageDrone* USummonDamageDrone::CreateAbility(float cooldown, AMech_RPGCharacter* owner)
{
	USummonDamageDrone* ability = NewObject<USummonDamageDrone>(StaticClass());
	ability->SetCooldown(cooldown);
	ability->owner = owner;
	ability->AddTag(selfTargetted, 1);
	return ability;
}

FString USummonDamageDrone::GetTooltipText()
{
	return "Summon Damage Drone" + UMiscLibrary::lnBreak + "Summons a drone with a basic weapon and ability and low health." + UMiscLibrary::lnBreak + "Cooldown: " + FString::SanitizeFloat(GetCooldown());
}

bool USummonDamageDrone::OnCooldown()
{
	if (!Super::OnCooldown() 
		&& droneSummoned != nullptr 
		&& !droneSummoned->IsDead())
	{
		return true;
	}

	return Super::OnCooldown();
}
