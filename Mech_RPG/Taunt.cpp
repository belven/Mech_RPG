
#pragma once
#include "Mech_RPG.h"
#include "Taunt.h"
#include "Mech_RPGCharacter.h"
#include "Mech_RPGPlayerController.h"
#include "BaseAIController.h"

bool UTaunt::Activate(class AMech_RPGCharacter* target, FVector targetLocation) {
	bool targetFound = false;

	if (target && target->GetGroup()) {
		for (AMech_RPGCharacter* member : target->GetGroup()->GetMembers()) {
			if (UMiscLibrary::IsCharacterAlive(member)) {
				ABaseAIController* con = Cast<ABaseAIController>(member->GetController());
				if (con && con->GetTarget() != owner) {
					con->SetTarget(owner);
					targetFound = true;
				}
			}
		}
	}

	if (targetFound) {
		SetOnCooldown(owner->GetWorld());
		return true;
	}
	return false;
}

UTaunt* UTaunt::CreateAbility(float cooldown, AMech_RPGCharacter* owner) {
	UTaunt* ability = NewObject<UTaunt>(StaticClass());
	ability->SetCooldown(cooldown);
	ability->owner = owner;
	ability->affectedTeam = AOEEnums::Enemy;
	return ability;
}