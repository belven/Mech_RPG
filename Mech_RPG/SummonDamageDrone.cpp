// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "AllyAIController.h"
#include "Drone.h"
#include "SummonDamageDrone.h"



bool USummonDamageDrone::Activate(class AMech_RPGCharacter* target, FVector targetLocation) {
	if (droneSummoned != nullptr && droneSummoned->IsDead()) {
		FNavLocation nav;
		owner->GetWorld()->GetNavigationSystem()->GetRandomPointInNavigableRadius(owner->GetActorLocation(), 200, nav);
		droneSummoned->SetActorLocation(nav.Location);
		SetOnCooldown(owner->GetWorld());
	}
	else {
		FNavLocation nav;
		int count = 0;

		while (droneSummoned == nullptr && count < 10) {
			owner->GetWorld()->GetNavigationSystem()->GetRandomPointInNavigableRadius(owner->GetActorLocation(), 200, nav);
			droneSummoned = UMiscLibrary::SpawnCharacter<ADrone>(owner->GetWorld(), nav.Location, owner->GetActorRotation(), ADrone::StaticClass());
			count++;
		}

		if (droneSummoned != nullptr) {
			droneSummoned->SetGroup(owner->GetGroup());
			droneSummoned->CreatePresetRole(GroupEnums::DPS);
			SetOnCooldown(owner->GetWorld());
			droneSummoned->team = owner->team;
		}
	}

	if (droneSummoned != nullptr) {
		FVector loc;
		loc = droneSummoned->GetActorLocation();
		loc.Z += (droneSummoned->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight());
		droneSummoned->SetActorLocation(loc);
		droneSummoned->BeginPlay();
		return true;
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
