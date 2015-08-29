// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "Mech_RPG.h"
#include "Engine.h"
#include "Mech_RPGPlayerController.h"
#include "Mech_RPGCharacter.h"
#include "AI/Navigation/NavigationSystem.h"
#include "BaseAIController.h"

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

	if (GetOwner()){
		if (GetOwner()->GetDemandedController() != NULL) {
			ABaseAIController* con = Cast<ABaseAIController>(GetOwner()->GetDemandedController());
			AMech_RPGCharacter* other = con->GetOwner();

			GetOwner()->SetDemandedController(NULL);
			other->SetDemandedController(NULL);

			if (con){
				con->Possess(GetOwner());
				Possess(other);
			}
		}

		if (!GetOwner()->IsDead()){

			if (swapWeapons) {
				GetOwner()->SwapWeapon();
				swapWeapons = false;
			}

			if (bAttackTarget && !IsTargetValid()){
				target = NULL;
			}
			else if (bAttackTarget){
				AttackTarget(DeltaTime);
			}

			if (bMoveToMouseCursor)
			{
				OnAttackReleased();
				MoveToMouseCursor();
			}
		}
		else {
			for (int i = 0; i < GetOwner()->GetGroup()->GetMembers().Num(); i++){
				AMech_RPGCharacter* character = GetOwner()->GetGroup()->GetMembers()[i];

				if (!character->IsDead()){
					SwapCharacter(i + 1);
				}
			}

			UnPossess();
			GetOwner()->Destroy();
		}
	}
}

void AMech_RPGPlayerController::AttackTarget(float DeltaTime) {
	AWeapon* weapon = owner->GetCurrentWeapon();

	if (weapon){
		bool targetInRange = false;

		float dist = FVector::Dist(owner->GetActorLocation(), target->GetActorLocation());

		if (dist <= weapon->GetRange()) {
			if (weapon->CanFire()){
				weapon->Fire(target, GetOwner());
			}

			targetInRange = true;
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

	InputComponent->BindAction("One", IE_Released, this, &AMech_RPGPlayerController::CharacterOne);
	InputComponent->BindAction("Two", IE_Released, this, &AMech_RPGPlayerController::CharacterTwo);
	InputComponent->BindAction("Three", IE_Released, this, &AMech_RPGPlayerController::CharacterThree);
	InputComponent->BindAction("Four", IE_Released, this, &AMech_RPGPlayerController::CharacterFour);

	InputComponent->BindAction("SwapWeapons", IE_Released, this, &AMech_RPGPlayerController::SwapWeapons);
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
	GetTargetUnderCursor();
	if (IsTargetValid()) {
		bAttackTarget = true;
	}
}

void AMech_RPGPlayerController::OnAttackReleased()
{
	target = NULL;
	bAttackTarget = false;
	DefaultMouseCursor = EMouseCursor::Hand;
}

void AMech_RPGPlayerController::GetTargetUnderCursor(){
	static FHitResult Hit;
	FCollisionQueryParams collision;
	collision.AddIgnoredActor(owner);

	Hit = GetHitFromCursor();

	if (Hit.bBlockingHit)
	{
		static AActor* targetFound;
		targetFound = Hit.GetActor();

		if (targetFound
			&& targetFound != owner
			&& targetFound->GetClass()
			&& targetFound->GetClass()->IsChildOf(AMech_RPGCharacter::StaticClass()))
		{
			target = Cast<AMech_RPGCharacter>(targetFound);
		}
	}
}

bool AMech_RPGPlayerController::IsTargetValid(){
	return target && !target->IsDead() && !target->CompareGroup(owner);
}

void AMech_RPGPlayerController::SwapCharacter(int index){
	// Do we have an owner
	if (GetOwner()) {
		UGroup* group = GetOwner()->GetGroup();

		// Does the owner have a group and is there more than 1 other person
		if (group && group->GetMembers().Num() > 1){

			// Does the character we want to swawp to exist
			if (group->GetMembers().Num() > index - 1){
				AMech_RPGCharacter* character = group->GetMembers()[index - 1];

				// Are we swapping to ourselves
				if (GetOwner() != character && !character->IsDead()){
					character->SetDemandedController(this);

					GetOwner()->SetDemandedController(character->GetController());
				}
			}
		}
	}
}


FHitResult AMech_RPGPlayerController::GetHitFromCursor(){
	static FHitResult Hit;
	GetHitResultUnderCursor(ECollisionChannel::ECC_Pawn, false, Hit);
	return  Hit;
}

AMech_RPGCharacter* AMech_RPGPlayerController::GetOwner(){
	return owner;
}


void AMech_RPGPlayerController::SetOwner(AMech_RPGCharacter* newVal){
	owner = newVal;
}

void AMech_RPGPlayerController::CharacterFour(){
	SwapCharacter(4);
}

void AMech_RPGPlayerController::CharacterOne(){
	SwapCharacter(1);
}

void AMech_RPGPlayerController::CharacterThree(){
	SwapCharacter(3);
}

void AMech_RPGPlayerController::CharacterTwo(){
	SwapCharacter(2);
}


void AMech_RPGPlayerController::SwapWeapons(){
	if (GetOwner() && !GetOwner()->IsDead()){
		swapWeapons = true;
	}
}