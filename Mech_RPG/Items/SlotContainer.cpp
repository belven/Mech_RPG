// Fill out your copyright notice in the Description page of Project Settings.

#include "Mech_RPG.h"
#include "SlotContainer.h"

USlotContainer::USlotContainer() : Super() {
}

TArray<UItem*>& USlotContainer::GetItems() {
	return items;
}

void USlotContainer::SetItems(TArray<UItem*> newVal) {
	items = newVal;
}

UItem* USlotContainer::GetExistingItemWithSpace(UItem* inItem) {
	for (UItem* item : items) {
		// Finds the first item with space available and a matching name
		if (item != nullptr && item->GetName().Equals(inItem->GetName()) && item->HasSpace()) {
			return item;
		}
	}
	return nullptr;
}

/* Adds an item to the inventory, if it finds an item with less than StackSize it adds the amount
* else it will create a new item with the remaining amount and set the one found to StackSize
*
* @return the input item with the amount set to the remainder if any, i.e. if it's not 0 then the inventory was full
*/
UItem* USlotContainer::AddItem(UItem* itemToAdd) {
	if (HasSpace() && itemToAdd->GetAmount() <= itemToAdd->GetStackSize()) {
		items.Add(itemToAdd);
		UItem* tempItem = itemToAdd->Copy();
		tempItem->SetAmount(0);
		return tempItem;
	}
	else {
		UItem* existingItem = GetExistingItemWithSpace(itemToAdd);

		// Check all existing matching items to see if they have space
		while (itemToAdd->GetAmount() > 0 && existingItem != nullptr) {
			int amountToAdd = itemToAdd->GetAmount();
			existingItem->TakeFrom(itemToAdd);

			// Try to find another item to add to
			existingItem = GetExistingItemWithSpace(itemToAdd);
		}

		// Keep adding new items until we're either full or added all items
		while (itemToAdd->GetAmount() > 0 && HasSpace()) {
			// Make a new item
			UItem* newItem = itemToAdd->Copy();
			newItem->SetAmount(0);
			newItem->TakeFrom(itemToAdd);
			// Add the new item
			items.Add(newItem);
		}
	}

	return itemToAdd;
}

bool USlotContainer::HasSpace() {
	return items.Num() < GetMaxItemCount();
}

/* This will reduce the an items amount by the given item if found */
bool USlotContainer::RemoveItem(UItem* itemToRemove) {

	// Is there only one item of this type in out inventory
	bool isAtMax = GetItemAmount(itemToRemove->GetName()) == itemToRemove->GetStackSize();

	for (UItem* item : items) {

		// Find the first item with the same name and is either the only item or not at max stack size
		if (item->GetName().Equals(itemToRemove->GetName()) && (isAtMax || item->GetAmount() < item->GetStackSize())) {
			int32 amountLeft = item->GetAmount() - itemToRemove->GetAmount();

			// We don't remove items we don't have 
			if (amountLeft < 0) {
				return false;
			}
			else if (amountLeft == 0) { // Simply remove it if the amount left is 0
				items.Remove(item);
				return true;
			}
			else { // Just change the amount
				item->SetAmount(amountLeft);
				return true;
			}
		}
	}

	return false;
}

/* Returns the total amount of items for the given name */
int32 USlotContainer::GetItemAmount(FString name) {
	int32 total = 0;
	for (UItem* item : items) {
		if (item->GetName().Equals(name)) {
			total += item->GetAmount();
		}
	}
	return total;
}

/* Gets the maximum amount of items this inventory can have */
int32 USlotContainer::GetMaxItemCount() {
	return maxItemCount;
}


/* Sets the maximum amount of items this inventory can have */
void USlotContainer::SetMaxItemCount(int32 newVal) {
	maxItemCount = newVal;
}
