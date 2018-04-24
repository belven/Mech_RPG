// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "InventoryUI.h"
#include "ItemUI.h"
#include "MiscLibrary.h"
#include "Characters/Mech_RPGCharacter.h"
#include "Items/Item.h"
#include "Weapons/Weapon.h"
#include "Items/Armour.h"
#include "Abilities/Ability.h"
#include "Mech_RPGPlayerController.h"

void UInventoryUI::SetOwner(AMech_RPGCharacter* inOwner)
{
	owner = inOwner;
}

AMech_RPGCharacter * UInventoryUI::GetOwner()
{
	return owner;
}

TArray<UItemUI*>& UInventoryUI::GetItems() {
	return items;
}

TArray<UItemUI*>& UInventoryUI::GetSelectedItems()
{
	return selectetItems;
}

void UInventoryUI::SetItems(TArray<UItemUI*> val) {
	items = val;
}

void UInventoryUI::AddItem(UItemUI* item)
{
	items.Add(item);
}

void UInventoryUI::RemoveItem(UItemUI* item)
{
	items.Remove(item);
}

void UInventoryUI::SelectItem(UItemUI* item)
{
	selectetItems.Add(item);
	//UMiscLibrary::GetPlayerController()->SelectedItem(item->GetItem());
}

void UInventoryUI::DeselectItem(UItemUI* item)
{
	selectetItems.Remove(item);
}

void UInventoryUI::EquipItems()
{
	for (UItemUI* itemUI : GetSelectedItems()) {
		UItem* item = itemUI->GetItem();
		if (item != nullptr && item->GetClass()->IsChildOf(UWeapon::StaticClass())) {
			GetOwner()->SetCurrentWeapon(Cast<UWeapon>(item));
		}
		/*else if (item->GetClass()->IsChildOf(UArmour::StaticClass())) {
			GetOwner()->AddArmour(Cast<UArmour>(item));
		}
		else if (item->GetClass()->IsChildOf(UAbility::StaticClass())) {
			GetOwner()->AddAbility(Cast<UAbility>(item));
		}*/
		itemUI->DeselectItem();
	}
}