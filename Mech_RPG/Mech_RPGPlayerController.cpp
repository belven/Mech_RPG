// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "Mech_RPG.h"
#include "Mech_RPGPlayerController.h"
#include "Mech_RPGCharacter.h"
#include "AI/Navigation/NavigationSystem.h"

AMech_RPGPlayerController::AMech_RPGPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}

void AMech_RPGPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	if (bAttackTarget){

	}
	else if (bMoveToMouseCursor)
	{
		MoveToMouseCursor();
	}
}

void AMech_RPGPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAction("SetDestination", IE_Pressed, this, &AMech_RPGPlayerController::OnSetDestinationPressed);
	InputComponent->BindAction("SetDestination", IE_Released, this, &AMech_RPGPlayerController::OnSetDestinationReleased);

	InputComponent->BindAction("Attack", IE_Pressed, this, &AMech_RPGPlayerController::OnAttackPressed);
	InputComponent->BindAction("Attack", IE_Released, this, &AMech_RPGPlayerController::OnAttackReleased);

}

void AMech_RPGPlayerController::MoveToMouseCursor()
{
	// Trace to see what is under the mouse cursor
	FHitResult Hit;

	GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, Hit);

	if (Hit.bBlockingHit)
	{
		// We hit something, move there
		SetNewMoveDestination(Hit.ImpactPoint);
	}
}
void AMech_RPGPlayerController::SetNewMoveDestination(const FVector DestLocation)
{
	APawn* const Pawn = GetPawn();
	if (Pawn)
	{
		UNavigationSystem* const NavSys = GetWorld()->GetNavigationSystem();
		float const Distance = FVector::Dist(DestLocation, Pawn->GetActorLocation());

		// We need to issue move command only if far enough in order for walk animation to play correctly
		if (NavSys && (Distance > 120.0f))
		{
			NavSys->SimpleMoveToLocation(this, DestLocation);
		}
	}
}

void AMech_RPGPlayerController::OnSetDestinationPressed()
{
	// set flag to keep updating destination until released
	bMoveToMouseCursor = true;
}

void AMech_RPGPlayerController::OnSetDestinationReleased()
{
	// clear flag to indicate we should stop updating the destination
	bMoveToMouseCursor = false;
}

void AMech_RPGPlayerController::OnAttackPressed()
{
	FHitResult Hit;

	GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, Hit);

	if (Hit.bBlockingHit)
	{
		FVector traceTo = Hit.ImpactPoint;
		FCollisionQueryParams collision;

		GetWorld()->LineTraceSingle(Hit, GetCharacter()->GetActorLocation() * 100, traceTo, ECC_Camera, collision);
		AActor* targetFound = Hit.GetActor();

		if (targetFound && targetFound->GetClass()->IsChildOf(AMech_RPGCharacter::StaticClass())){
			target = Cast<AMech_RPGCharacter>(targetFound);
			bAttackTarget = true;
		}
	}
}

void AMech_RPGPlayerController::OnAttackReleased()
{
	bAttackTarget = false;
}