///////////////////////////////////////////////////////////
//  AItem.cpp
//  Implementation of the Class AItem
//  Created on:      04-May-2015 11:16:11
//  Original author: sam
///////////////////////////////////////////////////////////
#include "Mech_RPG.h"
#include "Item.h"
#include "Weapons/Weapon.h"
#include "Armour.h"
#include "Characters/Mech_RPGCharacter.h"
#include "MiscLibrary.h"

int32 UItem::HighestItemLevel = 100;

#define mGetRandomEnum(end) UMiscLibrary::GetRandomEnum(end)

void UItem::CloneItemSettings(UItem* cloneFromItem)
{
	SetName(cloneFromItem->GetName());
	SetGrade(cloneFromItem->GetGrade());
	SetQuality(cloneFromItem->GetQuality());
	SetType(cloneFromItem->GetType());
	SetItemOwner(cloneFromItem->GetItemOwner());
}

UItem::UItem() : Super()
{
	SetGrade(1);
}

UItem* UItem::CreateItemByType(AMech_RPGCharacter* inOwner, ItemEnumns::ItemType type, int32 grade, int32 quality)
{
	switch (type)
	{
	case ItemEnumns::Weapon:
		return UWeapon::CreatePresetWeapon(inOwner, mGetRandomEnum(WeaponEnums::End), grade, quality);
	case ItemEnumns::Armour:
		return UArmour::CreateArmour("Test", UArmour::GetDeafultValue(mGetRandomEnum(ArmourGrades::End)), mGetRandomEnum(EArmourPosition::End), inOwner, grade, quality);
	}
	return nullptr;
}


ItemEnumns::ItemType UItem::GetType()
{
	return type;
}

void UItem::SetType(ItemEnumns::ItemType newVal)
{
	type = newVal;
}

FString UItem::GetName()
{
	return name;
}

void UItem::SetName(FString newVal)
{
	name = newVal;
}

void UItem::TakeFrom(UItem* otherItem)
{
	// Get the amount of items needed to add
	int amountToAdd = otherItem->GetAmount();

	// Can we just add to this item
	if (GetRemainingSpace() >= amountToAdd)
	{
		SetAmount(GetAmount() + otherItem->GetAmount());
		otherItem->SetAmount(0);
	}
	else
	{
		// If we're greater than stack size just set the amount to max size
		otherItem->SetAmount(otherItem->GetAmount() - GetRemainingSpace());
		SetAmount(GetStackSize());
	}
}

int32 UItem::GetGrade()
{
	return grade;
}

void UItem::SetGrade(int32 newVal)
{
	grade = newVal;
}

AMech_RPGCharacter* UItem::GetItemOwner()
{
	return itemOwner;
}

int32 UItem::GetQuality()
{
	return quality;
}

void UItem::SetQuality(int32 newQuality)
{
	quality = newQuality;
}

void UItem::SetItemOwner(AMech_RPGCharacter* newVal)
{
	itemOwner = newVal;
}

bool UItem::HasSpace()
{
	return GetAmount() < GetStackSize();
}

int32 UItem::GetRemainingSpace()
{
	return GetStackSize() - GetAmount();
}

int32 UItem::GetAmount()
{
	return amount;
}

void UItem::SetAmount(int32 newVal)
{
	amount = newVal;
}

int32 UItem::GetStackSize()
{
	return stackSize;
}

void UItem::SetStackSize(int32 newVal)
{
	stackSize = newVal;
}

UItem* UItem::Copy()
{
	return CreateItem(GetItemOwner(), GetName(), GetAmount(), GetGrade(), GetQuality(), GetStackSize());
}

FString UItem::GetTooltipText()
{
	return "";
}

UItem* UItem::CreateItem(AMech_RPGCharacter* inOwner, FString inName /*= "Test"*/, int32 inAmount /*= 1*/, int32 inGrade /*= 0*/, int32 inQuality /*= 0*/, int32 inStackSize /*= 3*/)
{
	UItem* newItem = NewObject<UItem>(StaticClass());
	newItem->SetName(inName);
	newItem->SetAmount(inAmount);
	newItem->SetStackSize(inStackSize);
	newItem->SetGrade(inGrade);
	newItem->SetItemOwner(inOwner);
	newItem->SetQuality(inQuality);
	return  newItem;
}
