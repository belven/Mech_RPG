// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Mech_RPG.h"
#include "MechPart.h"
#include "Characters/Mech_RPGCharacter.h"

UMechPart::UMechPart() : Super()
{
	meshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MechAttachment_MeshComponent"));
	SetActorHiddenInGame(true);
}

void UMechPart::SetItemOwner(AMech_RPGCharacter* inOwner)
{
	if (mesh == nullptr && meshClass != nullptr)
	{
		mesh = inOwner->GetWorld()->SpawnActor<UStaticMesh>(meshClass);
	}

	if (mesh != nullptr)
	{
		meshComponent->SetStaticMesh(mesh);
		meshComponent->SetMobility(EComponentMobility::Movable);
		meshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void UMechPart::SetActorHiddenInGame(bool bNewHidden)
{
	meshComponent->SetHiddenInGame(bNewHidden, true);
	meshComponent->SetVisibility(!bNewHidden, true);
}

