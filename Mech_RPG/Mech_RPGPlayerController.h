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
	bool swapWeapons;

public:
	AMech_RPGPlayerController();
	AMech_RPGCharacter* GetOwner();
	void SetOwner(AMech_RPGCharacter* newVal);
	void GetTargetUnderCursor();
	bool IsTargetValid();
	void DemandSwapCharacter(int index);
	FHitResult GetHitFromCursor();
	void CharacterFour();
	void CharacterOne();
	void CharacterThree();
	void CharacterTwo();
	void SwapWeapons();
	void ActivateAbility();
	void PlayerDied();
	void SwapCharacter();
	bool IsOwnerValid();

protected:
	/**
	 * True if the controlled character should navigate to the mouse cursor.
	 */
	uint32 bMoveToMouseCursor : 1;
	/**
	 * True if the controlled character should attack thier target.
	 */
	uint32 bAttackTarget : 1;

	AMech_RPGCharacter* target;

	/**
	 * Begin PlayerController interface
	 */
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;

	/**
	 * Navigate player to the current mouse cursor location.
	 */
	void MoveToMouseCursor();

	/**
	 * Navigate player to the given world location.
	 */
	void SetNewMoveDestination(const FVector DestLocation);

	/**
	 * Input handlers for SetDestination action.
	 */
	void OnSetDestinationPressed();
	void OnSetDestinationReleased();

	/**
	 * Input handlers for Attack action.
	 */
	void OnAttackPressed();
	void OnAttackReleased();

	void AttackTarget(float DeltaTime);
};