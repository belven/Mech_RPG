// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "InventoryUI.h"
#include "Mech_RPGCharacter.h"




void UInventoryUI::SetOwner(AMech_RPGCharacter* inOwner)
{
	owner = inOwner;
}

AMech_RPGCharacter * UInventoryUI::GetOwner()
{
	return owner;
}
