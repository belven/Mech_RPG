// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "MechPart.h"
#include "ItemPickup.h"
#include "Mech_RPGCharacter.h"

AItemPickup::AItemPickup() : Super()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> sniper(TEXT("/Game/TopDown/Meshes/Sniper"));
	if (sniper.Succeeded()) {
		mesh = sniper.Object;
		GetStaticMeshComponent()->SetStaticMesh(mesh);
	}
	GetStaticMeshComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GetStaticMeshComponent()->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	SetMobility(EComponentMobility::Movable);
}

void AItemPickup::Interact_Implementation(class AMech_RPGCharacter* character)
{
	if (item != nullptr && character->AddItem(item)->GetAmount() == 0) {
		SetActorHiddenInGame(true);
		Destroy();
	}
	// No Item so remove anyway
	else {
		SetActorHiddenInGame(true);
		Destroy();
	}
}

AItemPickup* AItemPickup::CreateItemPickup(AItem* item)
{
	AItemPickup* newItem = item->GetOwner()->GetWorld()->SpawnActor<AItemPickup>(AItemPickup::StaticClass());
	newItem->item = item;

	if (item->GetClass()->IsChildOf(AMechPart::StaticClass())) {
		newItem->GetStaticMeshComponent()->SetStaticMesh(Cast<AMechPart>(item)->mesh);
	}
	return newItem;
}

void AItemPickup::NotifyActorBeginCursorOver()
{

}
