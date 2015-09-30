#pragma once
#include "GameFramework/PlayerController.h"
#include "Mech_RPGPlayerController.generated.h"

class AMech_RPGCharacter;

UCLASS()
class AMech_RPGPlayerController : public APlayerController {
	GENERATED_BODY()
private:
	class AMech_RPGCharacter* owner;
	bool swapWeapons;
	bool altPressed;
	bool ctrlPressed;
	bool shiftPressed;

	FCollisionQueryParams collision;
	FCollisionObjectQueryParams objectCollision;
	FHitResult hit;
public:
	AMech_RPGPlayerController(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "Owner")
	AMech_RPGCharacter* GetOwner();

	void SetOwner(AMech_RPGCharacter* newVal);

	bool IsMechCharacter(AActor* character);
	bool IsCover(AActor* character);

	AMech_RPGCharacter* GetTargetUnderCursor();
	FHitResult GetHitFromCursor();

	bool IsTargetValid(AMech_RPGCharacter* inTarget);

	void DemandSwapCharacter(int index);

	void CharacterFour();
	void CharacterOne();
	void CharacterThree();
	void CharacterTwo();

	void SwapWeapons();
	void ActivateAbility();
	void PlayerDied();
	void SwapCharacter();
	bool IsOwnerValid();

	void AllyAbility(int index);
	void AllyAttack(int index);
	void AllyMove(int index);

	void GroupAttack();

	void AltPressed();
	void AltReleased();

	void CtrlPressed();
	void CtrlReleased();

	void ShiftPressed();
	void ShiftReleased();

	void PerformAllyCommand(int index);
	AMech_RPGCharacter* GetTarget();
	void SetTarget(AMech_RPGCharacter* newVal);


	void MoveToActor(AActor* target);
	void MoveToLocation(FVector location);

	void ZoomIn();
	void ZoomOut();
	void ResetZoom();
	//void UpdateRotation();
	TArray<AMech_RPGCharacter*> GetCharactersInRange(float range);

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
	AMech_RPGCharacter* cursorTarget;

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
	void FireWeapon(AActor* hit);
	void SetupCollision();
};