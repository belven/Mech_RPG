// Fill out your copyright notice in the Description page of Project Settings.

#include "Mech_RPG.h"
#include "MechAttachment.h"


/**
 * Sets default values for this actor's properties
 */
AMechAttachment::AMechAttachment()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMechAttachment::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMechAttachment::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

