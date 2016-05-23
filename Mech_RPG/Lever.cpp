// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "Lever.h"
#include "DoorInteractable.h"


ALever::ALever() : Super() {

}

void ALever::Interact_Implementation(AMech_RPGCharacter * character)
{
	if (GetDoor() != nullptr) {
		GetDoor()->Interact(character);
	}
}
