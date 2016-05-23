// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "DoorInteractable.h"

void ADoorInteractable::Interact_Implementation(class AMech_RPGCharacter* character)
{
	if (open) {
		Close();
		open = false;
	}
	else {
		Open();
		open = true;
	}
}
