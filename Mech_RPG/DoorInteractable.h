// Copyright of Explosive Industries

#pragma once

#include "Interactable.h"
#include "DoorInteractable.generated.h"

/**
 * 
 */
UCLASS()
class MECH_RPG_API ADoorInteractable : public AInteractable
{
	GENERATED_BODY()
	
private:
	bool canBeUsed = false;
public:

	virtual void Interact_Implementation(class AMech_RPGCharacter* character);
	
};
