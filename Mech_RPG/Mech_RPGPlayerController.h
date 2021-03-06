#pragma once
#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h"
#include "UI/InventoryUI.h"
#include "CollisionChannels.h"
#include "UI/CharacterStats.h"
#include "Mech_RPGPlayerController.generated.h"

class AMech_RPGCharacter;
class UQuestDisplayUI;

UENUM(BlueprintType)
namespace PlayerControllerEnums {
	enum LastUsed {
		None,
		Ability,
		NPCInteract,
		Interactable
	};
}

UENUM(BlueprintType)
namespace TargetEnums {
	enum MouseOverTarget {
		Character,
		ItemPickup,
		Interactable
	};
}

UCLASS()
class AMech_RPGPlayerController : public APlayerController {
	GENERATED_BODY()
private:
	class AMech_RPGCharacter* owner;
	bool swapWeapons;
	bool altPressed;
	bool ctrlPressed;
	bool shiftPressed;
	bool panLeft;
	bool panRight;
	bool panUp;
	bool panDown;

	bool characterPaneOpen = false;
	bool inventoryOpen = false;

	FCollisionQueryParams collision;
	FCollisionObjectQueryParams objectCollision;

	class AInteractable* lastTargetInteractable;

	// The last character we targeted with an ability or weapon, used so we can move to it if we can't see it
	class AMech_RPGCharacter* lastCharacterTarget;
	UAbility* lastUsedAbility;
	UItem* selectedItem;

	PlayerControllerEnums::LastUsed lastAction;

	float interactionRange = 200;

	~AMech_RPGPlayerController();

public:
	AMech_RPGPlayerController(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "Owner")
		AMech_RPGCharacter* GetPlayerControllerOwner();

	void SetPlayerControllerOwner(AMech_RPGCharacter* newVal);

	void Possess(APawn* InPawn) override;

	bool IsMechCharacter(AActor* character);
	bool IsInteractable(AActor * character);

	AMech_RPGCharacter* GetTargetUnderCursor();
	class AInteractable* GetInteractableUnderCursor();

	FHitResult GetHitFromCursor(ECollisionChannel channel = mCharacterCollision);

	bool IsTargetValid(AMech_RPGCharacter* inTarget);

	void DemandSwapCharacter(int index);

	virtual	void BeginPlay() override;

	void GenerateUI();

	void CharacterFive();
	void CharacterFour();
	void CharacterOne();
	void CharacterThree();
	void CharacterTwo();

	void SwapWeapons();
	void ActivateAbility();

	FVector GetCharacterLocation(AMech_RPGCharacter* tempCharacter);

	// Attempts to use an ability and returns the result
	bool usedAbility(UAbility* ability, FVector location, AMech_RPGCharacter* tempCharacter);

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


	void MoveToActor(AActor* targetActor);
	void MoveToLocation(FVector location);

	void ZoomIn();
	void ZoomOut();
	void ResetZoom();

	void PanLeft();
	void PanLeftReleased();

	void PanRight();
	void PanRightReleased();

	void PanUp();
	void PanUpReleased();

	void PanDown();
	void PanDownReleased();

	//void UpdateRotation();
	TArray<AMech_RPGCharacter*> GetCharactersInRange(float range);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = UI)
		TSubclassOf<UUserWidget> characterPaneTemplate;
	TSubclassOf<UUserWidget> inventoryTemplate;
	TSubclassOf<UUserWidget> questListTemplate;
	
	UUserWidget* characterPane;
	UInventoryUI* inventoryUI;
	UQuestDisplayUI* questList;

	void OpenCharacterPane();
	void OpenInventory();

	void MouseOverActor(AActor* inActor, TEnumAsByte<TargetEnums::MouseOverTarget> targeType);
	virtual void NotifyActorEndCursorOver() override;
	virtual void NotifyActorBeginCursorOver() override;

	UFUNCTION(BlueprintCallable, Category = "Events")
		UItem* GetSelectedItem() { return selectedItem; }

	UFUNCTION(BlueprintCallable, Category = "Events")
		void SelectedItem(UItem* val) { selectedItem = val; }
protected:
	/**
	 * True if the controlled character should navigate to the mouse cursor.
	 */
	uint32 bMoveToMouseCursor : 1;
	/**
	 * True if the controlled character should attack their target.
	 */
	uint32 bAttackTarget : 1;

	AMech_RPGCharacter* target;
	AMech_RPGCharacter* cursorTarget;
	FVector lastTargetLocation;

	/**
	 * Begin PlayerController interface
	 */
	virtual void PlayerTick(float DeltaTime) override;
	
	void SetCursorType();

	void CalculateActions(float DeltaTime);

	void PerformPanning();

	virtual void SetupInputComponent() override;

	void Reload();

	void PauseToggle();
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
	void OnRightClickPressed();
	void OnRightClickReleased();

	void AttackTarget(float DeltaTime);
	void FireWeapon(AActor* hit);
	void SetupCollision();

	UFUNCTION(BlueprintCallable, Category = "Events")
		void AddQuest(UQuest * newQuest);

	UFUNCTION(BlueprintCallable, Category = "Events")
		void PlayerItemPickup(UItem* item);

	UFUNCTION(BlueprintCallable, Category = "Events")
		void PlayerSwappedWeapons(UWeapon* oldWeapon, UWeapon* newWeapon);
};