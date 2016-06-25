// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "Equipable.h"

void AEquipable::Equip()
{
	if (GetOwner() != nullptr && GetOwner()->GetMesh() != nullptr) {
		AttachRootComponentTo(GetOwner()->GetMesh(), "RightHand");
	}

	SetActorHiddenInGame(false);

	SetEquipped(true);
}

void AEquipable::Unequip()
{
	meshComponent->DetachFromParent();	
	SetActorHiddenInGame(true);
	SetEquipped(false);
}
