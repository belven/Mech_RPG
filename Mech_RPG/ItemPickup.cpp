// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "ItemPickup.h"

void AItemPickup::Interact_Implementation(class AMech_RPGCharacter* character)
{
	if (character->AddItem(item)->GetAmount() == 0) {
		Destroy();
	}
}

AItemPickup* AItemPickup::CreateItemPickup(AItem* item)
{
	AItemPickup* newItem = item->GetOwner()->GetWorld()->SpawnActor<AItemPickup>(AItemPickup::StaticClass());
	newItem->item = item;
	return newItem;
}
