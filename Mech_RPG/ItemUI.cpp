// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "ItemUI.h"
#include "Item.h"
#include "InventoryUI.h"


void UItemUI::SetItem(AItem* inItem)
{
	owner = inItem;
}

AItem* UItemUI::GetItem()
{
	return owner;
}

UInventoryUI* UItemUI::GetInventory() {
	return inventory;
}

void UItemUI::SetInventory(UInventoryUI* val) {
	inventory = val;
}