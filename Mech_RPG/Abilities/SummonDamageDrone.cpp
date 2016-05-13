// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "AllyAIController.h"
#include "Drone.h"
#include "SummonDamageDrone.h"



bool USummonDamageDrone::Activate(class AMech_RPGCharacter* target, FVector targetLocation) {
	/*if (droneSummoned != nullptr && droneSummoned->IsDead()) {
		FNavLocation nav;
		owner->GetWorld()->GetNavigationSystem()->GetRandomPointInNavigableRadius(owner->GetActorLocation(), 400, nav);
		droneSummoned->SetActorLocation(nav.Location);
		SetOnCooldown(owner->GetWorld());
	}
	else*/

	if (droneSummoned != nullptr && droneSummoned->IsDead()) {
		droneSummoned->Destroy();
		droneSummoned->GetGroup()->RemoveMember(droneSummoned);
		droneSummoned = nullptr;
	}

	if (droneSummoned == nullptr ) {
		FNavLocation nav;
		int count = 0;	

		while (droneSummoned == nullptr && count < 10) {
			owner->GetWorld()->GetNavigationSystem()->GetRandomPointInNavigableRadius(owner->GetActorLocation(), 200, nav);
			FVector loc;
			loc = nav.Location;
			//loc.Z += (droneSummoned->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight());
			droneSummoned = UMiscLibrary::SpawnCharacter<ADrone>(owner->GetWorld(), loc, owner->GetActorRotation(), ADrone::StaticClass());
			count++;
		}

		if (droneSummoned != nullptr) {
			droneSummoned->SetGroup(owner->GetGroup());
			droneSummoned->CreatePresetRole(GroupEnums::DPS);
			droneSummoned->team = owner->team;
			droneSummoned->SetDead(false);
			SetOnCooldown(owner->GetWorld());
			return true;
		}
	}
	return false;
}


USummonDamageDrone* USummonDamageDrone::CreateAbility(float cooldown, AMech_RPGCharacter* owner) {
	USummonDamageDrone* ability = NewObject<USummonDamageDrone>(StaticClass());
	ability->SetCooldown(cooldown);
	ability->owner = owner;
	ability->AddTag(needsTargetTag, 0.0F);
	return ability;
}

FString USummonDamageDrone::GetTooltipText()
{
	return "Summon Damage Drone" + UMiscLibrary::lnBreak + "Summons a drone with a basic weapon and ability and low health." + UMiscLibrary::lnBreak + "Cooldown: " + FString::SanitizeFloat(GetCooldown());
}
