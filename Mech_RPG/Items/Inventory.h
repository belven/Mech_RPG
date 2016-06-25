///////////////////////////////////////////////////////////
//  Inventory.h
//  Implementation of the Class UInventory
//  Created on:      04-May-2015 11:13:50
//  Original author: sam
///////////////////////////////////////////////////////////

#pragma once
#include "SlotContainer.h"
#include "Inventory.generated.h"

UCLASS(Blueprintable)
class UInventory : public USlotContainer
{
	GENERATED_BODY()

public:
	UInventory();

};
