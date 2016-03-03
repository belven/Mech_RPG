// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "ItemUI.h"
#include "Item.h"


void UItemUI::SetItem(AItem* inItem)
{
	owner = inItem;
}

AItem* UItemUI::GetItem()
{
	return owner;
}

