// Fill out your copyright notice in the Description page of Project Settings.

#include "Mech_RPG.h"
#include "SlotContainer.h"

USlotContainer::USlotContainer() : Super() {
}

TArray<AItem*>& USlotContainer::GetItems() {
	return items;
}

void USlotContainer::SetItems(TArray<AItem*> newVal) {
	items = newVal;
}

/* Adds an item to the inventory, if it finds an item with less than StackSize it adds the amount
* else it will create a new item with the remaining amount and set the one found to StackSize
*
* @return the input item with the amount set to the remainder if any, i.e. if it's not 0 then the inventory was full
*/
AItem* USlotContainer::AddItem(AItem* itemToAdd) {

	AItem* tempitem = NULL;

	for (AItem* item : GetItems()) {
		// Finds the first item with space available and a matching name
		if (item != nullptr && item->GetName().Equals(itemToAdd->GetName()) && item->GetAmount() < item->GetStackSize()) {
			tempitem = item;
			break;
		}
	}

	// If we found an item
	if (tempitem != NULL) {
		int32 amountOfSpace = itemToAdd->GetStackSize() - itemToAdd->GetAmount();

		// If there is enough space left just add to it
		if (amountOfSpace >= 0) {
			tempitem->SetAmount(tempitem->GetAmount() + itemToAdd->GetAmount());
			itemToAdd->SetAmount(0);
		}
		else {
			// Otherwise max the amount of the current and add a new item to the inventory
			// At this stage will we need to check if their inventory is full and possibly add multiple items 
			if (HasSpace()) {

				// Get the space left in the current item
				int amount = tempitem->GetStackSize() - tempitem->GetAmount();
				tempitem->SetAmount(amount);

				// Set the amount to the amount left - the difference
				amount = itemToAdd->GetAmount() - amount;
				itemToAdd->SetAmount(amount);

				// While we still have items to add and space
				while (amount > 0 && HasSpace()) {

					// Make a new item
					AItem* newItem = tempitem->Copy();

					// If we're greater than stack size just set the amount to max size
					newItem->SetAmount(amount < tempitem->GetStackSize() ? amount : tempitem->GetStackSize());
					GetItems().Add(newItem);

					// Update the amount
					amount -= newItem->GetAmount();
				}

				// This may not always be 0, if not then the inventory was filled up with no more space
				itemToAdd->SetAmount(amount);
			}
		}

	}
	else if (HasSpace()) {
		// We didn't find an existing item so add it
		// At this stage will we need to check if their inventory is full
		GetItems().Add(itemToAdd->Copy());

		// Set item amount to 0 so we know that all the items have been taken
		itemToAdd->SetAmount(0);
	}

	return itemToAdd;
}

bool USlotContainer::HasSpace() {
	return GetItems().Num() < GetMaxItemCount();
}

/* This will reduce the an items amount by the given item if found */
bool USlotContainer::RemoveItem(AItem* itemToRemove) {

	// Is there only one item of this type in out inventory
	bool isAtMax = GetItemAmount(itemToRemove->GetName()) == itemToRemove->GetStackSize();

	for (AItem* item : GetItems()) {

		// Find the first item with the same name and is either the only item or not at max stack size
		if (item->GetName().Equals(itemToRemove->GetName()) && (isAtMax || item->GetAmount() < item->GetStackSize())) {
			int32 amountLeft = item->GetAmount() - itemToRemove->GetAmount();

			// We don't remove items we don't have 
			if (amountLeft < 0) {
				return false;
			}
			else if (amountLeft == 0) { // Simply remove it if the amount left is 0
				GetItems().Remove(item);
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
	for (AItem* item : GetItems()) {
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
