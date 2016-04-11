// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "Mech_RPGCharacter.h"
#include "Interactable.h"


// Sets default values
AInteractable::AInteractable()
{
	//GetStaticMeshComponent() = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Interactable_MeshComponent"));
	//SetRootComponent(GetStaticMeshComponent()); 
	SetMobility(EComponentMobility::Movable);
}

// Called when the game starts or when spawned
void AInteractable::BeginPlay()
{
	Super::BeginPlay();
		
	if (mesh != nullptr) {
		GetStaticMeshComponent()->SetStaticMesh(mesh);
		GetStaticMeshComponent()->SetMobility(EComponentMobility::Movable);
		//GetStaticMeshComponent()->bOwnerNoSee = false;
	//	GetStaticMeshComponent()->bCastDynamicShadow = true;
		//GetStaticMeshComponent()->CastShadow = true;
		GetStaticMeshComponent()->SetHiddenInGame(false);
		GetStaticMeshComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		GetStaticMeshComponent()->SetRelativeScale3D(FVector(10, 10, 10));
	}
}

void AInteractable::SetActorHiddenInGame(bool bNewHidden)
{
	Super::SetActorHiddenInGame(bNewHidden);
	GetStaticMeshComponent()->SetHiddenInGame(bNewHidden, true);
	GetStaticMeshComponent()->SetVisibility(!bNewHidden, true);
}
