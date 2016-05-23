// Copyright of Explosive Industries

#pragma once

#include "Interactable.h"
#include "Lever.generated.h"

class ADoorInteractable;

UCLASS()
class MECH_RPG_API ALever : public AInteractable
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere)
	ADoorInteractable* door;

public:
	ALever();

	virtual void Interact_Implementation(class AMech_RPGCharacter* character);

	UFUNCTION(BlueprintCallable, Category = "Lever")
	ADoorInteractable* GetDoor() const
	{
		return door;
	}

	UFUNCTION(BlueprintCallable, Category = "Lever")
	void SetDoor(ADoorInteractable* val)
	{
		door = val;
	}
};
