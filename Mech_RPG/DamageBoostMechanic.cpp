// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "DamageBoostMechanic.h"

void ADamageBoostMechanic::BeginPlay() {
	Super::BeginPlay();

	if (GetBoss() != NULL) {
		float modifier = GetBoss()->GetHealthChangeModifier() + boostAmount;
		GetBoss()->SetHealthChangeModifier(modifier);
	}
}

void ADamageBoostMechanic::SetDead(bool newVal) {
	AMech_RPGCharacter::SetDead(newVal);
	if (GetBoss() != NULL) {
		float modifier = GetBoss()->GetHealthChangeModifier() - boostAmount;
		GetBoss()->SetHealthChangeModifier(modifier);
	}
}