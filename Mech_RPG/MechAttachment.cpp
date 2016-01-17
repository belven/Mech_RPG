// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Mech_RPG.h"
#include "MechAttachment.h"
#include "Mech_RPGCharacter.h"


/**
 * Sets default values for this actor's properties
 */
AMechAttachment::AMechAttachment()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	meshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MechAttachment_MeshComponent"));

}

AMech_RPGCharacter * AMechAttachment::GetOwner()
{
	return owner;
}

void AMechAttachment::SetOwner(AMech_RPGCharacter* inOwner)
{
	owner = inOwner;

	if (mesh == nullptr && meshClass != nullptr) {
		mesh = GetWorld()->SpawnActor<UStaticMesh>(meshClass);
	}

	if (mesh != nullptr) {
		meshComponent->SetStaticMesh(mesh);

		meshComponent->bOwnerNoSee = false;
		meshComponent->bCastDynamicShadow = true;
		meshComponent->CastShadow = true;
		meshComponent->SetHiddenInGame(false);
		meshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		meshComponent->AttachTo(owner->GetMesh(), "RightHand");
	}
}

// Called when the game starts or when spawned
void AMechAttachment::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AMechAttachment::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

