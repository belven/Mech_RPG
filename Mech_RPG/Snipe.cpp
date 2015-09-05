#include "Mech_RPG.h"
#include "Snipe.h"
#include "Mech_RPGCharacter.h"

void USnipe::Activate(AMech_RPGCharacter* target, FVector targetLocation) {
	if (target) {
		target->Hit(owner, 1000);
		SetOnCooldown(owner->GetWorld());
	}
}

USnipe* USnipe::CreateAbility(float cooldown, AMech_RPGCharacter* owner) {
	USnipe* ability = NewObject<USnipe>(StaticClass());
	ability->SetCooldown(cooldown);
	ability->owner = owner;
	return ability;
}