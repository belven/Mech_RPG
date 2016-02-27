// Copyright of Explosive Industries

#pragma once

#include "GameFramework/Actor.h"
#include "Interactable.generated.h"

UCLASS()
class MECH_RPG_API AInteractable : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AInteractable();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Interact(class AMech_RPGCharacter* character) {
		check(0 && "You must override this");
	}

};
