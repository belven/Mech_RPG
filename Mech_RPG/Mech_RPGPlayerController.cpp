// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "Mech_RPG.h"
#include "Engine.h"
#include "Mech_RPGPlayerController.h"
#include "Mech_RPGCharacter.h"
#include "AI/Navigation/NavigationSystem.h"

AMech_RPGPlayerController::AMech_RPGPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Hand;
}

/**
 * Begin PlayerController interface
 */
void AMech_RPGPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	FHitResult Hit;
	FVector eyeLoc;
	FRotator eyeRot;



	if (bAttackTarget){
		AttackTarget(DeltaTime);
	}
	else if (bMoveToMouseCursor)
	{
		target = NULL;
		MoveToMouseCursor();
	}
	else if (!target) {
		owner->GetActorEyesViewPoint(eyeLoc, eyeRot);

		GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, Hit);

		if (Hit.bBlockingHit)
		{
			FVector traceTo = Hit.ImpactPoint;
			FCollisionQueryParams collision;

			GetWorld()->LineTraceSingle(Hit, eyeLoc * 20, traceTo, ECollisionChannel::ECC_Pawn, collision);

			AActor* targetFound = Hit.GetActor();

			if (targetFound &&targetFound != owner &&targetFound->GetClass()->IsChildOf(AMech_RPGCharacter::StaticClass())){
				target = Cast<AMech_RPGCharacter>(targetFound);
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, "Target Found");
				DefaultMouseCursor = EMouseCursor::Crosshairs;
			}
		}
	}
}

void AMech_RPGPlayerController::AttackTarget(float DeltaTime) {
	if (target && owner && owner->GetWeapons().Num() > 0){
		bool targetInRange = false;

		for (AWeapon* weapon : owner->GetWeapons()) {
			float dist = FVector::Dist(owner->GetActorLocation(), target->GetActorLocation());

			if (dist <= weapon->GetRange()) {
				if (weapon->CanFire(DeltaTime)){
					target->Hit(owner, weapon->GetDamage(), NULL);
				}

				targetInRange = true;
			}
		}

		if (!targetInRange && GetWorld()->GetNavigationSystem())
		{
			GetWorld()->GetNavigationSystem()->SimpleMoveToLocation(this, target->GetActorLocation());
		}
		else {
			StopMovement();
		}
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

/**
 * Navigate player to the current mouse cursor location.
 */
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
/**
 * Navigate player to the given world location.
 */
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

/**
 * Input handlers for SetDestination action.
 */
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

/**
 * Input handlers for Attack action.
 */
void AMech_RPGPlayerController::OnAttackPressed()
{
	if (target) {
		bAttackTarget = true;
	}
}

void AMech_RPGPlayerController::OnAttackReleased()
{
	bAttackTarget = false;
	DefaultMouseCursor = EMouseCursor::Hand;
}


AMech_RPGCharacter* AMech_RPGPlayerController::GetOwner(){
	return owner;
}


void AMech_RPGPlayerController::SetOwner(AMech_RPGCharacter* newVal){
	owner = newVal;
}