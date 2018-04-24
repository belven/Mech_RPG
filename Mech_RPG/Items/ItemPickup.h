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
	UPROPERTY(EditAnywhere, Category = "PickUp")
		UItem* item;
public:
	AItemPickup();
	
	virtual void Interact_Implementation(class AMech_RPGCharacter* character);

	static AItemPickup* CreateItemPickup(UItem* item);

	virtual void NotifyActorBeginCursorOver() override;
	
};
