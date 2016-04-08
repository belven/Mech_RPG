// Copyright of Explosive Industries

#pragma once

#include "Interactable.h"
#include "Item.h"
#include "ItemPickup.generated.h"

/**
 * 
 */
UCLASS()
class MECH_RPG_API AItemPickup : public AInteractable
{
	GENERATED_BODY()
private:


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PickUp")
		AItem* item;
	
	virtual void Interact_Implementation(class AMech_RPGCharacter* character);

	AItemPickup* CreateItemPickup(AItem* item);
	
};
