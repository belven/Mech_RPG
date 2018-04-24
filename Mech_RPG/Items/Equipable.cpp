// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "Equipable.h"
#include "Characters/Mech_RPGCharacter.h"

void UEquipable::Equip()
{
	//meshComponent->AttachToComponent(GetItemOwner()->GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false), "RightHand");
	SetActorHiddenInGame(false);
	SetEquipped(true);
}

void UEquipable::Unequip()
{
	//meshComponent->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepRelative, true));
	SetActorHiddenInGame(true);
	SetEquipped(false);
}
