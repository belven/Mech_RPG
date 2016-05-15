// Copyright of Explosive Industries

#pragma once

#include "GameFramework/Actor.h"
#include "Interactable.generated.h"

UCLASS()
class MECH_RPG_API AInteractable : public AStaticMeshActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AInteractable();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void SetActorHiddenInGame(bool bNewHidden) override;

	UFUNCTION(BlueprintNativeEvent, Category = "Interactable")
		void Interact(class AMech_RPGCharacter* character);

	virtual void Interact_Implementation(class AMech_RPGCharacter* character) {
		check(0 && "You must override this");
	}
protected:
	UStaticMesh* mesh = nullptr;
};
