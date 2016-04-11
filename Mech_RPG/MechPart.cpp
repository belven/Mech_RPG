// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Mech_RPG.h"
#include "MechPart.h"
#include "Mech_RPGCharacter.h"


/**
 * Sets default values for this actor's properties
 */
AMechPart::AMechPart() : Super()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	meshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MechAttachment_MeshComponent"));
	SetRootComponent(meshComponent);
}

void AMechPart::SetOwner(AMech_RPGCharacter* inOwner)
{
	Super::SetOwner(inOwner);

	if (mesh == nullptr && meshClass != nullptr) {
		mesh = GetWorld()->SpawnActor<UStaticMesh>(meshClass);
	}

	if (mesh != nullptr) {
		meshComponent->SetStaticMesh(mesh);
		meshComponent->SetMobility(EComponentMobility::Movable);
		meshComponent->bOwnerNoSee = false;
		meshComponent->bCastDynamicShadow = true;
		meshComponent->CastShadow = true;
		meshComponent->SetHiddenInGame(false);
		meshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		if (inOwner != nullptr) {
			AttachRootComponentTo(owner->GetMesh(), "RightHand");
		}
	}
}

// Called when the game starts or when spawned
void AMechPart::BeginPlay()
{
	Super::BeginPlay();

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

