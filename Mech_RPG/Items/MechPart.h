// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Item.h"
#include "MechPart.generated.h"

UCLASS(Blueprintable)
class MECH_RPG_API UMechPart : public UItem
{
	GENERATED_BODY()
private:

public:
	// Sets default values for this actor's properties
	UMechPart();

	void SetItemOwner(AMech_RPGCharacter* inOwner) override;

	virtual void SetActorHiddenInGame(bool bNewHidden);
	UPROPERTY()
		UStaticMesh* mesh = nullptr;
protected:
	UPROPERTY()
		UStaticMeshComponent* meshComponent = nullptr;

};
