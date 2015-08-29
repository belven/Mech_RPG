#include "Mech_RPG.h"
#include "Snipe.h"
#include "Mech_RPGCharacter.h"

void USnipe::Activate(AMech_RPGCharacter* owner, AMech_RPGCharacter* target, FVector targetLocation) {
	if (target) {
		target->Hit(owner, 1000);
		SetOnCooldown(owner->GetWorld());
	}
}