// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "ItemUI.h"
#include "Items/Item.h"
#include "InventoryUI.h"


void UItemUI::SetItem(UItem* inItem)
{
	owner = inItem;
}

UItem* UItemUI::GetItem()
{
	return owner;
}

UInventoryUI* UItemUI::GetInventory() {
	return inventory;
}

void UItemUI::SetInventory(UInventoryUI* val) {
	inventory = val;
}