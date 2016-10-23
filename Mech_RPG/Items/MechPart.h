// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Item.h"
#include "MechPart.generated.h"

UCLASS(Blueprintable)
class MECH_RPG_API AMechPart : public AItem
{
	GENERATED_BODY()
private:

public:	
	// Sets default values for this actor's properties
	AMechPart();
	~AMechPart();

	virtual	void SetItemOwner(AMech_RPGCharacter* inOwner);
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	virtual void SetActorHiddenInGame(bool bNewHidden) override;
	UStaticMesh* mesh = nullptr;
protected:
	UStaticMeshComponent* meshComponent = nullptr;
	TSubclassOf<UStaticMesh> meshClass = nullptr;
	
};
