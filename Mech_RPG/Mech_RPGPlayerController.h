// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/PlayerController.h"
#include "Mech_RPGPlayerController.generated.h"

UCLASS()
class AMech_RPGPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMech_RPGPlayerController();

protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	/** True if the controlled character should attack thier target. */
	uint32 bAttackTarget : 1;

	class AMech_RPGCharacter* target;

	// Begin PlayerController interface
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	// End PlayerController interface

	/** Navigate player to the current mouse cursor location. */
	void MoveToMouseCursor();
		
	/** Navigate player to the given world location. */
	void SetNewMoveDestination(const FVector DestLocation);

	/** Input handlers for SetDestination action. */
	void OnSetDestinationPressed();
	void OnSetDestinationReleased();

	/** Input handlers for Attack action. */
	void OnAttackPressed();
	void OnAttackReleased();


};


