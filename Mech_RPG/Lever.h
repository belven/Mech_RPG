// Copyright of Explosive Industries

#pragma once

#include "Interactable.h"
#include "Lever.generated.h"

/**
 * 
 */
UCLASS()
class MECH_RPG_API ALever : public AInteractable
{
	GENERATED_BODY()
public:
	ALever();
	
	virtual void Interact(class AMech_RPGCharacter* character);
};
