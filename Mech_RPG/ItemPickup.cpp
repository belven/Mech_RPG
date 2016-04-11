// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "ItemPickup.h"

AItemPickup::AItemPickup() : Super()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> sniper(TEXT("/Game/TopDown/Meshes/Sniper"));
	if (sniper.Succeeded()) {
		mesh = sniper.Object;
		GetStaticMeshComponent()->SetStaticMesh(mesh);
	}
}

void AItemPickup::Interact_Implementation(class AMech_RPGCharacter* character)
{
	if (item != nullptr && character->AddItem(item)->GetAmount() == 0) {
		Destroy();
	}
}

AItemPickup* AItemPickup::CreateItemPickup(AItem* item)
{
	AItemPickup* newItem = item->GetOwner()->GetWorld()->SpawnActor<AItemPickup>(AItemPickup::StaticClass());
	newItem->item = item;
	return newItem;
}
