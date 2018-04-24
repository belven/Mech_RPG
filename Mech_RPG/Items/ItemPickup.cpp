// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "MechPart.h"
#include "ItemPickup.h"
#include "Characters/Mech_RPGCharacter.h"

AItemPickup::AItemPickup() : Super()
{
	/*static ConstructorHelpers::FObjectFinder<UStaticMesh> sniper(TEXT("/Game/TopDown/Meshes/Sniper"));
	if (sniper.Succeeded())
	{
		mesh = sniper.Object;
		GetStaticMeshComponent()->SetStaticMesh(mesh);
	}*/
	GetStaticMeshComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GetStaticMeshComponent()->SetCollisionObjectType(mItemCollision);
	SetMobility(EComponentMobility::Movable);
}

void AItemPickup::Interact_Implementation(class AMech_RPGCharacter* character)
{
	if (item != nullptr && character->AddItem(item)->GetAmount() == 0)
	{
		SetActorHiddenInGame(true);
		Destroy();
	}
	// No Item so remove anyway
	else if (item == nullptr)
	{
		SetActorHiddenInGame(true);
		Destroy();
	}
}

AItemPickup* AItemPickup::CreateItemPickup(UItem* item)
{
	AItemPickup* newItem = item->GetItemOwner()->GetWorld()->SpawnActor<AItemPickup>(AItemPickup::StaticClass());
	newItem->item = item;

	if (item->GetClass()->IsChildOf(UMechPart::StaticClass()))
	{
		newItem->GetStaticMeshComponent()->SetStaticMesh(Cast<UMechPart>(item)->mesh);
	}
	return newItem;
}

void AItemPickup::NotifyActorBeginCursorOver()
{

}
