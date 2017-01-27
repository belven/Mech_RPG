// Copyright of Explosive Industries
#pragma once
#include "Mech_RPG.h"
#include "Test/Cover.h"


// Sets default values
ACover::ACover()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetActorEnableCollision(true);	
}

// Called when the game starts or when spawned
void ACover::BeginPlay()
{
	Super::BeginPlay();
	currentHealth = coverMaxHealth;
}

// Called every frame
void ACover::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void ACover::ChangeHealth(FHealthChange damage) {
	currentHealth -= damage.changeAmount;

	if (currentHealth <= 0) {
		SetActorHiddenInGame(true);
		SetActorEnableCollision(false);
	}
}
