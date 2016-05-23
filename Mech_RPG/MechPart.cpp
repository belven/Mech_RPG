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
	PrimaryActorTick.bCanEverTick = true;

	meshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MechAttachment_MeshComponent"));
	//meshComponent->Owner = this;
	SetRootComponent(meshComponent);
	meshComponent->SetHiddenInGame(true);
}

AMechPart::~AMechPart()
{
	//meshComponent->DestroyComponent(true);
}

void AMechPart::SetOwner(AMech_RPGCharacter* inOwner)
{
	Super::SetOwner(inOwner);

	if (mesh == nullptr && meshClass != nullptr) {
		mesh = GetWorld()->SpawnActor<UStaticMesh>(meshClass);
	}

	if (mesh != nullptr) {
		meshComponent->SetStaticMesh(mesh);
		//meshComponent->SetMobility(EComponentMobility::Movable);
		meshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		if (inOwner != nullptr && inOwner->GetMesh() != nullptr) {
			AttachRootComponentTo(inOwner->GetMesh(), "RightHand");
		}
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

	if (!bNewHidden && GetOwner() != nullptr && GetOwner()->GetMesh() != nullptr) {
		AttachRootComponentTo(GetOwner()->GetMesh(), "RightHand");
	}
	else if (GetOwner() != nullptr && GetOwner()->GetMesh() != nullptr)
	{
		meshComponent->DetachFromParent();
	}
}

