// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Mech_RPG.h"
#include "MechPart.h"
#include "Characters/Mech_RPGCharacter.h"


/**
 * Sets default values for this actor's properties
 */
AMechPart::AMechPart() : Super()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	meshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MechAttachment_MeshComponent"));
	//meshComponent->Owner = this;
	SetRootComponent(meshComponent);
	SetActorHiddenInGame(true);
}

AMechPart::~AMechPart()
{
	//meshComponent->DestroyComponent(true);
}

void AMechPart::SetItemOwner(AMech_RPGCharacter* inOwner)
{
	Super::SetItemOwner(inOwner);

	if (mesh == nullptr && meshClass != nullptr) {
		mesh = GetWorld()->SpawnActor<UStaticMesh>(meshClass);
	}

	if (mesh != nullptr) {
		meshComponent->SetStaticMesh(mesh);
		meshComponent->SetMobility(EComponentMobility::Movable);
		meshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}


// Called every frame
void AMechPart::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMechPart::SetActorHiddenInGame(bool bNewHidden)
{
	Super::SetActorHiddenInGame(bNewHidden);
	GetRootComponent()->SetHiddenInGame(bNewHidden, true);
	meshComponent->SetHiddenInGame(bNewHidden, true);
	meshComponent->SetVisibility(!bNewHidden, true);
}

