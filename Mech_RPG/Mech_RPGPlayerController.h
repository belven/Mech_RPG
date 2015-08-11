// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/PlayerController.h"
#include "Mech_RPGPlayerController.generated.h"

class AMech_RPGCharacter;

UCLASS()
class AMech_RPGPlayerController : public APlayerController
{
	GENERATED_BODY()
private:
	class AMech_RPGCharacter* owner;


public:
	AMech_RPGPlayerController();
	AMech_RPGCharacter* GetOwner();
	void SetOwner(AMech_RPGCharacter* newVal);

protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	/** True if the controlled character should attack thier target. */
	uint32 bAttackTarget : 1;

	AMech_RPGCharacter* target;

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

	void AttackTarget(float DeltaTime);

};


