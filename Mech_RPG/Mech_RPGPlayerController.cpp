#pragma once
#include "Mech_RPG.h"
#include "Engine.h"
#include "Mech_RPGPlayerController.h"
#include "AllyAIController.h"
#include "Characters/Mech_RPGCharacter.h"
#include "UI/QuestDisplayUI.h"
#include "Interactable.h"
#include "UI/ItemUI.h"
#include "Weapons.h"
#include "Quests/QuestManager.h"

#define mCanSeeLocation(location) UMiscLibrary::CanSee(GetPlayerControllerOwner()->GetWorld(), GetPlayerControllerOwner()->GetActorLocation(), location)

AMech_RPGPlayerController::~AMech_RPGPlayerController()
{
	UMiscLibrary::SetPlayerGroup(nullptr);
}

AMech_RPGPlayerController::AMech_RPGPlayerController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bShowMouseCursor = true;
	bAttackTarget = false;
	DefaultMouseCursor = EMouseCursor::Hand;
	objectCollision.AddObjectTypesToQuery(mWorldCollision);
	objectCollision.AddObjectTypesToQuery(mCharacterCollision);

	static ConstructorHelpers::FClassFinder<UUserWidget> characterPaneClass(TEXT("/Game/TopDown/Blueprints/UI/CharacterUI/Character_Pane.Character_Pane_C"));

	if (characterPaneClass.Class != nullptr)
	{
		characterPaneTemplate = characterPaneClass.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> inventoryClass(TEXT("/Game/TopDown/Blueprints/UI/ItemsUI/Inventory.Inventory_C"));

	if (inventoryClass.Class != nullptr)
	{
		inventoryTemplate = inventoryClass.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> questListClass(TEXT("/Game/TopDown/Blueprints/UI/QuestList.QuestList_C"));

	if (questListClass.Class != nullptr)
	{
		questListTemplate = questListClass.Class;
	}

	SetActorTickEnabled(true);
	PrimaryActorTick.bCanEverTick = true;
	bEnableMouseOverEvents = true;

}

void AMech_RPGPlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (characterPaneTemplate != nullptr)
	{
		characterPane = CreateWidget<UUserWidget>(this, characterPaneTemplate);
		characterPane->AddToViewport();
		characterPane->SetVisibility(ESlateVisibility::Hidden);
	}

	if (inventoryTemplate != nullptr)
	{
		inventory = CreateWidget<UInventoryUI>(this, inventoryTemplate);
		inventory->AddToViewport();
		inventory->SetVisibility(ESlateVisibility::Hidden);
		inventory->SetPositionInViewport(FVector2D(600, 100));
	}

	if (questListTemplate != nullptr)
	{
		questList = CreateWidget<UQuestDisplayUI>(this, questListTemplate);
		questList->AddToViewport();
		questList->SetVisibility(ESlateVisibility::Visible);
		questList->SetPositionInViewport(FVector2D(1200, 100));
	}
}

void  AMech_RPGPlayerController::PerformPanning()
{
	if (panLeft || panRight)
	{
		FRotator rot = GetPlayerControllerOwner()->CameraBoom->RelativeRotation;
		rot.Yaw += panLeft ? -1 : 1;
		GetPlayerControllerOwner()->CameraBoom->SetRelativeRotation(rot);
		GetPlayerControllerOwner()->UpdateHealthBarRot();
	}

	if (panUp || panDown)
	{
		FRotator rot = GetPlayerControllerOwner()->CameraBoom->RelativeRotation;
		rot.Pitch += panUp ? -1 : 1;

		if (rot.Pitch < -85)
		{
			rot.Pitch = -85;
		}
		else if (rot.Pitch > 0)
		{
			rot.Pitch = 0;
		}

		GetPlayerControllerOwner()->CameraBoom->SetRelativeRotation(rot);
		GetPlayerControllerOwner()->UpdateHealthBarRot();
	}
}

/**
 * Begin PlayerController interface
 */
void AMech_RPGPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	//Do we have an owner
	if (GetPlayerControllerOwner())
	{

		// Is our owner demanding a character change
		if (GetPlayerControllerOwner()->GetDemandedController() != nullptr)
		{
			SwapCharacter();
		}

		PerformPanning();

		// Is our owner is still alive
		if (!GetPlayerControllerOwner()->IsDead())
		{
			cursorTarget = GetTargetUnderCursor();

			SetCursorType();
			CalculateActions(DeltaTime);
		}
		else
		{
			PlayerDied();
		}
	}
}

void AMech_RPGPlayerController::SetCursorType()
{
	if (cursorTarget != nullptr && !cursorTarget->IsDead())
	{
		CurrentMouseCursor = EMouseCursor::Crosshairs;
	}
	else if (GetInteractableUnderCursor() != nullptr)
	{
		CurrentMouseCursor = EMouseCursor::GrabHand;
	}
	else
	{
		CurrentMouseCursor = EMouseCursor::Hand;
	}
}

void AMech_RPGPlayerController::CalculateActions(float DeltaTime)
{
	// Are we trying to interact with an Interactable
	if (lastAction == PlayerControllerEnums::Interactable)
	{
		if (mCanSeeLocation(lastTargetInteractable->GetActorLocation())
			&& GetPlayerControllerOwner()->GetDistanceTo(lastTargetInteractable) <= interactionRange)
		{
			lastAction = PlayerControllerEnums::None;
			GetPlayerControllerOwner()->Interact(lastTargetInteractable);
			inventory->GenerateInventory();
		}
		else
		{
			MoveToActor(lastTargetInteractable);
		}
	}
	// Are we trying to use an ability
	else if (lastAction == PlayerControllerEnums::Ability && lastUsedAbility != nullptr)
	{
		ActivateAbility();
	}
	// Are we trying to interact with a character
	else if (lastAction == PlayerControllerEnums::NPCInteract)
	{
		// Are we requesting a new target to attack			
		if (bAttackTarget && IsTargetValid(cursorTarget))
		{
			target = cursorTarget;
			AttackTarget(DeltaTime);
		}
		// Is our current target valid
		else if (IsTargetValid(target))
		{
			AttackTarget(DeltaTime);
		}
		// No valid target so stop firing
		else
		{
			GetPlayerControllerOwner()->OnStopFiring.Broadcast();
		}

		//if (UMiscLibrary::IsCharacterAlive(cursorTarget) && cursorTarget->CompareGroup(GetOwner())) {
		//	if (mCanSee(cursorTarget->GetActorLocation())
		//		&& GetOwner()->GetDistanceTo(cursorTarget) <= interactionRange) {
		//		lastAction = PlayerControllerEnums::None;
		//		GetOwner()->NPCInteract(cursorTarget);
		//	}
		//	else {
		//		MoveToActor(cursorTarget);
		//	}
		//}
	}
}

void AMech_RPGPlayerController::MoveToActor(AActor* targetActor)
{
	if (GetPlayerControllerOwner()->CanMove())
	{
		GetPlayerControllerOwner()->OnStopFiring.Broadcast();
		GetWorld()->GetNavigationSystem()->SimpleMoveToActor(this, targetActor);
	}
}

void AMech_RPGPlayerController::MoveToLocation(FVector location)
{
	if (GetPlayerControllerOwner()->CanMove())
	{
		GetPlayerControllerOwner()->OnStopFiring.Broadcast();
		GetWorld()->GetNavigationSystem()->SimpleMoveToLocation(this, location);
	}
}

void AMech_RPGPlayerController::AttackTarget(float DeltaTime)
{
	// Are we targeting ourselves
	if (target == GetPlayerControllerOwner())
	{
		FireWeapon(nullptr);
		GetPlayerControllerOwner()->LookAt(target);
	}
	// Have we traced to another character or cover
	else if (mCanSeeLocation(target->GetActorLocation()))
	{
		FireWeapon(target);
		GetPlayerControllerOwner()->LookAt(target);
	}
	// We've hit some scenery so move towards the target
	else if (GetWorld()->GetNavigationSystem())
	{
		MoveToLocation(target->GetActorLocation());
	}
}

void AMech_RPGPlayerController::SetupCollision()
{
	collision.ClearIgnoredComponents();

	if (owner->GetGroup() != nullptr && owner->GetGroup()->HasMemebers())
	{
		for (AMech_RPGCharacter* member : owner->GetGroup()->GetMembers())
		{
			if (member != target && member != cursorTarget)
			{
				collision.AddIgnoredActor(member);
			}
		}
	}
}

void AMech_RPGPlayerController::AddQuest(UQuest * newQuest)
{
	if (questList != nullptr)
	{
		questList->GenerateQuests();
	}
}

void AMech_RPGPlayerController::PlayerItemPickup(UItem* item)
{
	inventory->GenerateInventory();
}

void AMech_RPGPlayerController::PlayerSwappedWeapons(UWeapon* oldWeapon, UWeapon* newWeapon)
{
	//characterPane->UpdateWeaponBar();
}

TArray<AMech_RPGCharacter*> AMech_RPGPlayerController::GetCharactersInRange(float range)
{
	return UMiscLibrary::GetCharactersInRange(range, GetPlayerControllerOwner());
}

void AMech_RPGPlayerController::FireWeapon(AActor* hit)
{
	UWeapon* weapon = GetPlayerControllerOwner()->GetCurrentWeapon();
	float distToTarget = FVector::Dist(GetPlayerControllerOwner()->GetActorLocation(), target->GetActorLocation());

	// Are we in weapons range
	if (weapon != nullptr && distToTarget <= weapon->GetRange())
	{
		if (GetPlayerControllerOwner()->CanAttack() && weapon->CanFire())
		{
			weapon->UseWeapon(target);
		}

		owner->GetGroup()->GroupMemberHit(target, owner);
		StopMovement();
	}
	// We're out of range so move closer
	else if (GetWorld()->GetNavigationSystem())
	{
		MoveToLocation(target->GetActorLocation());
	}
}

void AMech_RPGPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("SetDestination", IE_Pressed, this, &AMech_RPGPlayerController::OnSetDestinationPressed);
	InputComponent->BindAction("SetDestination", IE_Released, this, &AMech_RPGPlayerController::OnSetDestinationReleased);

	InputComponent->BindAction("Attack", IE_Pressed, this, &AMech_RPGPlayerController::OnRightClickPressed);
	InputComponent->BindAction("Attack", IE_Released, this, &AMech_RPGPlayerController::OnRightClickReleased);

	InputComponent->BindAction("GroupAttack", IE_Pressed, this, &AMech_RPGPlayerController::GroupAttack);

	InputComponent->BindAction("One", IE_Released, this, &AMech_RPGPlayerController::CharacterOne);
	InputComponent->BindAction("Two", IE_Released, this, &AMech_RPGPlayerController::CharacterTwo);
	InputComponent->BindAction("Three", IE_Released, this, &AMech_RPGPlayerController::CharacterThree);
	InputComponent->BindAction("Four", IE_Released, this, &AMech_RPGPlayerController::CharacterFour);
	InputComponent->BindAction("Five", IE_Released, this, &AMech_RPGPlayerController::CharacterFive);

	InputComponent->BindAction("SwapWeapons", IE_Released, this, &AMech_RPGPlayerController::SwapWeapons);
	InputComponent->BindAction("ActivateAbility", IE_Released, this, &AMech_RPGPlayerController::ActivateAbility);

	InputComponent->BindAction("Shift", IE_Pressed, this, &AMech_RPGPlayerController::ShiftPressed);
	InputComponent->BindAction("Shift", IE_Released, this, &AMech_RPGPlayerController::ShiftReleased);

	InputComponent->BindAction("Alt", IE_Pressed, this, &AMech_RPGPlayerController::AltPressed);
	InputComponent->BindAction("Alt", IE_Released, this, &AMech_RPGPlayerController::AltReleased);

	InputComponent->BindAction("Ctrl", IE_Pressed, this, &AMech_RPGPlayerController::CtrlPressed);
	InputComponent->BindAction("Ctrl", IE_Released, this, &AMech_RPGPlayerController::CtrlReleased);
	InputComponent->BindAction("ZoomIn", IE_Pressed, this, &AMech_RPGPlayerController::ZoomIn);
	InputComponent->BindAction("ZoomOut", IE_Pressed, this, &AMech_RPGPlayerController::ZoomOut);

	InputComponent->BindAction("PanLeft", IE_Pressed, this, &AMech_RPGPlayerController::PanLeft);
	InputComponent->BindAction("PanLeft", IE_Released, this, &AMech_RPGPlayerController::PanLeftReleased);

	InputComponent->BindAction("PanRight", IE_Pressed, this, &AMech_RPGPlayerController::PanRight);
	InputComponent->BindAction("PanRight", IE_Released, this, &AMech_RPGPlayerController::PanRightReleased);

	InputComponent->BindAction("PanUp", IE_Pressed, this, &AMech_RPGPlayerController::PanUp);
	InputComponent->BindAction("PanUp", IE_Released, this, &AMech_RPGPlayerController::PanUpReleased);

	InputComponent->BindAction("PanDown", IE_Pressed, this, &AMech_RPGPlayerController::PanDown);
	InputComponent->BindAction("PanDown", IE_Released, this, &AMech_RPGPlayerController::PanDownReleased);

	InputComponent->BindAction("ResetZoom", IE_Pressed, this, &AMech_RPGPlayerController::ResetZoom);
	InputComponent->BindAction("CharacterPane", IE_Pressed, this, &AMech_RPGPlayerController::OpenCharacterPane);
	InputComponent->BindAction("Inventory", IE_Pressed, this, &AMech_RPGPlayerController::OpenInventory);

	InputComponent->BindAction("Reload", IE_Pressed, this, &AMech_RPGPlayerController::Reload);
}

void AMech_RPGPlayerController::Reload()
{
	if (UMiscLibrary::IsCharacterAlive(GetPlayerControllerOwner())
		&& GetPlayerControllerOwner()->GetCurrentWeapon() != nullptr)
	{
		UWeapon* weapon = GetPlayerControllerOwner()->GetCurrentWeapon();

		if (mIsChildOf(weapon, UMagazineWeapon::StaticClass()))
		{
			UMagazineWeapon* mWeapon = Cast<UMagazineWeapon>(weapon);
			mWeapon->Reload();
		}
	}
}

void AMech_RPGPlayerController::OpenCharacterPane()
{
	if (characterPane->GetVisibility() == ESlateVisibility::Visible)
	{
		//FInputModeGameAndUI data;
		//SetInputMode(data);
		//SetPause(false);
		characterPaneOpen = false;
		characterPane->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		//FInputModeUIOnly data;
		//SetInputMode(data);
		//SetPause(true);
		characterPaneOpen = true;
		characterPane->SetVisibility(ESlateVisibility::Visible);
	}
}

void AMech_RPGPlayerController::OpenInventory()
{
	if (inventory->GetVisibility() == ESlateVisibility::Visible)
	{
		//FInputModeGameAndUI data;
		//SetInputMode(data);
		//SetPause(false);
		inventoryOpen = false;
		inventory->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		//FInputModeUIOnly data;
		//SetInputMode(data);
		//SetPause(true);
		inventoryOpen = true;
		inventory->SetVisibility(ESlateVisibility::Visible);
	}
}

void AMech_RPGPlayerController::MouseOverActor(AActor* inActor, TEnumAsByte<TargetEnums::MouseOverTarget> targeType)
{
	/*switch (targeType)
	{
	case TargetEnums::Character:
		CurrentMouseCursor = EMouseCursor::Crosshairs;
		break;
	case TargetEnums::Interactable:
		CurrentMouseCursor = EMouseCursor::GrabHand;
		break;
	case TargetEnums::ItemPickup:
		CurrentMouseCursor = EMouseCursor::GrabHandClosed;
		break;
	default:
		CurrentMouseCursor = EMouseCursor::Hand;
		break;
	}*/
}

void AMech_RPGPlayerController::NotifyActorEndCursorOver()
{
	CurrentMouseCursor = EMouseCursor::Hand;
}

void AMech_RPGPlayerController::NotifyActorBeginCursorOver()
{

}

/**
 * Navigate player to the current mouse cursor location.
 */
void AMech_RPGPlayerController::MoveToMouseCursor()
{
	FHitResult Hit;

	GetHitResultUnderCursor(mWorldCollision, false, Hit);

	//if (Hit.bBlockingHit) {
	SetNewMoveDestination(Hit.ImpactPoint);
	//}
}

/**
 * Navigate player to the given world location.
 */
void AMech_RPGPlayerController::SetNewMoveDestination(const FVector DestLocation)
{
	UNavigationSystem* const NavSys = GetWorld()->GetNavigationSystem();

	if (UMiscLibrary::IsCharacterAlive(GetPlayerControllerOwner()) && NavSys != nullptr)
	{
		MoveToLocation(DestLocation);
	}
}

/**
 * Input handlers for SetDestination action.
 */
void AMech_RPGPlayerController::OnSetDestinationPressed()
{
	bMoveToMouseCursor = true;

	target = nullptr;
	lastCharacterTarget = nullptr;
	lastTargetInteractable = nullptr;
	lastUsedAbility = nullptr;
	lastAction = PlayerControllerEnums::None;
	OnRightClickReleased();
	MoveToMouseCursor();
}

void AMech_RPGPlayerController::OnSetDestinationReleased()
{
	bMoveToMouseCursor = false;
}

void AMech_RPGPlayerController::ZoomIn()
{
	if (GetPlayerControllerOwner() != nullptr) GetPlayerControllerOwner()->CameraBoom->TargetArmLength += 100;
}

void AMech_RPGPlayerController::ZoomOut()
{
	if (GetPlayerControllerOwner() != nullptr) GetPlayerControllerOwner()->CameraBoom->TargetArmLength -= 100;
}

void AMech_RPGPlayerController::ResetZoom()
{
	if (GetPlayerControllerOwner() != nullptr)
	{
		GetPlayerControllerOwner()->CameraBoom->TargetArmLength = 1700;
		FRotator rot = GetPlayerControllerOwner()->CameraBoom->RelativeRotation;
		rot.Yaw = GetPlayerControllerOwner()->GetViewRotation().Yaw;
		rot.Pitch = -75.f;
		GetPlayerControllerOwner()->CameraBoom->SetRelativeRotation(rot);
	}
}

void AMech_RPGPlayerController::PanLeft()
{
	if (GetPlayerControllerOwner() != nullptr)
	{
		panLeft = true;
		FRotator rot = GetPlayerControllerOwner()->CameraBoom->RelativeRotation;
		rot.Yaw -= 18;
		GetPlayerControllerOwner()->CameraBoom->SetRelativeRotation(rot);
	}
}

void AMech_RPGPlayerController::PanLeftReleased()
{
	panLeft = false;
}

void AMech_RPGPlayerController::PanRight()
{
	panRight = true;
}

void AMech_RPGPlayerController::PanRightReleased()
{
	panRight = false;
}

void AMech_RPGPlayerController::PanUp()
{
	panUp = true;
}

void AMech_RPGPlayerController::PanUpReleased()
{
	panUp = false;
}

void AMech_RPGPlayerController::PanDown()
{
	panDown = true;
}

void AMech_RPGPlayerController::PanDownReleased()
{
	panDown = false;
}

void AMech_RPGPlayerController::OnRightClickPressed()
{
	if (GetInteractableUnderCursor() != nullptr)
	{
		lastAction = PlayerControllerEnums::Interactable;
		lastTargetInteractable = GetInteractableUnderCursor();
	}
	else
	{
		bAttackTarget = true;
		lastAction = PlayerControllerEnums::NPCInteract;
	}
}

void AMech_RPGPlayerController::OnRightClickReleased()
{
	bAttackTarget = false;
}

AMech_RPGCharacter* AMech_RPGPlayerController::GetTargetUnderCursor()
{
	FHitResult Hit;
	Hit = GetHitFromCursor();

	if (Hit.bBlockingHit)
	{
		AActor* targetFound = Hit.GetActor();

		if (targetFound != nullptr && IsMechCharacter(targetFound))
		{
			return Cast<AMech_RPGCharacter>(targetFound);
		}
	}
	return nullptr;
}

AInteractable* AMech_RPGPlayerController::GetInteractableUnderCursor()
{
	FHitResult Hit;
	Hit = GetHitFromCursor();

	if (Hit.bBlockingHit || (Hit = GetHitFromCursor(mItemCollision)).bBlockingHit)
	{
		AActor* targetFound = Hit.GetActor();

		if (targetFound != nullptr && IsInteractable(targetFound))
		{
			return Cast<AInteractable>(targetFound);
		}
	}
	return nullptr;
}

bool AMech_RPGPlayerController::IsMechCharacter(AActor* character)
{
	return character->GetClass()->IsChildOf(AMech_RPGCharacter::StaticClass());
}

bool AMech_RPGPlayerController::IsInteractable(AActor* character)
{
	return character->GetClass()->IsChildOf(AInteractable::StaticClass());
}

bool AMech_RPGPlayerController::IsTargetValid(AMech_RPGCharacter* inTarget)
{
	if (inTarget != nullptr && !inTarget->IsDead())
	{
		if (GetPlayerControllerOwner()->GetCurrentWeapon())
		{
			if (GetPlayerControllerOwner()->GetCurrentWeapon()->Heals())
			{
				return inTarget->CompareGroup(owner);
			}
			else
			{
				return !inTarget->CompareGroup(owner);
			}
		}
	}
	return false;
}

void AMech_RPGPlayerController::DemandSwapCharacter(int index)
{
	// Do we have an owner
	if (GetPlayerControllerOwner() != nullptr)
	{
		UGroup* group = GetPlayerControllerOwner()->GetGroup();

		// Does the owner have a group and is there more than 1 other person
		if (group != nullptr && group->GetMembers().Num() > 1)
		{

			// Does the character we want to swap to exist
			if (group->GetMembers().Num() > index - 1)
			{
				AMech_RPGCharacter* character = group->GetMembers()[index - 1];

				// Are we swapping to ourselves
				if (character != nullptr && GetPlayerControllerOwner() != character && !character->IsDead())
				{
					character->SetDemandedController(this);

					GetPlayerControllerOwner()->SetDemandedController(character->GetController());
				}
			}
		}
	}
}

FHitResult AMech_RPGPlayerController::GetHitFromCursor(ECollisionChannel channel)
{
	static FHitResult Hit;
	GetHitResultUnderCursor(channel, true, Hit);
	return Hit;
}

AMech_RPGCharacter* AMech_RPGPlayerController::GetPlayerControllerOwner()
{
	return owner;
}

void AMech_RPGPlayerController::SetPlayerControllerOwner(AMech_RPGCharacter* newVal)
{
	owner = newVal;
	inventory->SetOwner(owner);
	questList->SetCharacter(owner);
	questList->GenerateQuests();
	owner->OnQuestAdded.AddUniqueDynamic(this, &AMech_RPGPlayerController::AddQuest);
	owner->OnItemPickUpEvent.AddUniqueDynamic(this, &AMech_RPGPlayerController::PlayerItemPickup);
	inventory->GenerateInventory();

	for (UItemUI* item : inventory->GetSelectedItems())
	{
		item->DeselectItem();
	}
	inventory->GetSelectedItems().Empty();

}

void AMech_RPGPlayerController::Possess(APawn* InPawn)
{
	Super::Possess(InPawn);
	if (IsMechCharacter(InPawn))
	{
		SetPlayerControllerOwner(Cast<AMech_RPGCharacter>(InPawn));
		UMiscLibrary::SetPlayerController(this);
	}
}

void AMech_RPGPlayerController::CharacterFive()
{
	PerformAllyCommand(5);
}

void AMech_RPGPlayerController::CharacterFour()
{
	PerformAllyCommand(4);
}

void AMech_RPGPlayerController::CharacterOne()
{
	PerformAllyCommand(1);
}

void AMech_RPGPlayerController::CharacterThree()
{
	PerformAllyCommand(3);
}

void AMech_RPGPlayerController::CharacterTwo()
{
	PerformAllyCommand(2);
}

void AMech_RPGPlayerController::PerformAllyCommand(int index)
{
	if (shiftPressed)
	{
		AllyMove(index);
	}
	else if (altPressed)
	{
		AllyAbility(index);
	}
	else if (ctrlPressed)
	{
		AllyAttack(index);
	}
	else
	{
		DemandSwapCharacter(index);
	}
}

void AMech_RPGPlayerController::SwapWeapons()
{
	if (IsOwnerValid())
	{
		swapWeapons = true;
	}
}

FVector AMech_RPGPlayerController::GetCharacterLocation(AMech_RPGCharacter* tempCharacter)
{
	FVector location;

	if (!UMiscLibrary::IsCharacterAlive(tempCharacter))
	{
		location = GetHitFromCursor().ImpactPoint;
	}
	else if (UMiscLibrary::IsCharacterAlive(tempCharacter))
	{
		location = tempCharacter->GetActorLocation();
	}
	return location;
}

void AMech_RPGPlayerController::ActivateAbility()
{
	AMech_RPGCharacter* tempCharacter = cursorTarget;

	if (IsOwnerValid()
		&& GetPlayerControllerOwner()->HasAbilities()
		&& !GetPlayerControllerOwner()->Channelling()
		&& GetPlayerControllerOwner()->CanCast())
	{

		//SetupCollision();

		// Check if we've already tried to use an ability and we can use it, or try to use another one
		if (lastUsedAbility == nullptr)
		{
			for (UAbility* ability : GetPlayerControllerOwner()->GetAbilities())
			{
				if (usedAbility(ability, GetCharacterLocation(tempCharacter), tempCharacter))
				{
					break;
				}
			}
		}
		else
		{
			usedAbility(lastUsedAbility, lastTargetLocation, lastCharacterTarget);
		}
	}
}

bool AMech_RPGPlayerController::usedAbility(UAbility* ability, FVector location, AMech_RPGCharacter* tempCharacter)
{
	//Only use an ability if we have LoS to our target/location
	// and if the ability needs a target
	if (!ability->GetTagTrue(UAbility::selfTargetted) && !mCanSeeLocation(location))
	{
		MoveToLocation(location);
		lastUsedAbility = ability;
		lastCharacterTarget = tempCharacter;
		lastTargetLocation = location;
		lastAction = PlayerControllerEnums::Ability;
		return true;
	}

	// Check if the ability is valid.
	if (ability != nullptr
		&& !ability->OnCooldown()
		// Check if there is a target, is it valid for the attack?
		&& (tempCharacter == nullptr || UMiscLibrary::IsTargetValid(GetPlayerControllerOwner(), tempCharacter, ability->GetAffectedTeam())))
	{

		// If the ability doesn't need a target or we can see the location, try to use it 
		if (ability->Activate(tempCharacter, location))
		{
			GetPlayerControllerOwner()->SetCurrentAbility(ability);
			UQuestManager::AbilityUsed(ability);
			StopMovement();

			if (IsTargetValid(target))
			{
				lastAction = PlayerControllerEnums::NPCInteract;
			}
			else
			{
				lastAction = PlayerControllerEnums::None;
			}

			// Clear last used and target
			lastCharacterTarget = nullptr;
			lastUsedAbility = nullptr;
			lastTargetLocation = FVector::ZeroVector;
			return true;
		}
	}
	return false;
}

void AMech_RPGPlayerController::PlayerDied()
{
	for (int i = 0; i < GetPlayerControllerOwner()->GetGroup()->GetMembers().Num(); i++)
	{
		AMech_RPGCharacter* character = GetPlayerControllerOwner()->GetGroup()->GetMembers()[i];

		if (UMiscLibrary::IsCharacterAlive(character))
		{
			DemandSwapCharacter(i + 1);
			break;
		}
	}
}

void AMech_RPGPlayerController::SwapCharacter()
{
	AAllyAIController* aiCon = Cast<AAllyAIController>(GetPlayerControllerOwner()->GetDemandedController());
	AMech_RPGCharacter* aiOwner = aiCon->GetAIOwner();
	AMech_RPGCharacter* aiTarget = aiCon->GetTarget();

	GetPlayerControllerOwner()->SetDemandedController(nullptr);
	aiOwner->SetDemandedController(nullptr);

	if (aiCon != nullptr)
	{
		aiCon->SetTarget(GetTarget());
		aiCon->SetPlayerControlledLocation(FVector::ZeroVector);
		aiCon->Possess(GetPlayerControllerOwner());
		aiOwner->MirrorCamera(GetPlayerControllerOwner());
		Possess(aiOwner);
	}
}

bool AMech_RPGPlayerController::IsOwnerValid()
{
	return GetPlayerControllerOwner() != nullptr && !GetPlayerControllerOwner()->IsDead();
}

void AMech_RPGPlayerController::AllyAbility(int index)
{

}

void AMech_RPGPlayerController::AllyAttack(int index)
{
	UGroup* group = GetPlayerControllerOwner()->GetGroup();

	if (group != nullptr)
	{
		AMech_RPGCharacter* character = group->GetMember(index);
		if (UMiscLibrary::IsCharacterAlive(character) && character != GetPlayerControllerOwner())
		{
			AAllyAIController* con = Cast<AAllyAIController>(character->GetController());

			if (con->IsTargetValid(cursorTarget))
			{
				con->SetPlayerControlledLocation(FVector::ZeroVector);
				con->SetTarget(cursorTarget);
			}
		}
	}
}

void AMech_RPGPlayerController::GroupAttack()
{
	UGroup* group = GetPlayerControllerOwner()->GetGroup();

	if (group != nullptr)
	{
		for (AMech_RPGCharacter* character : group->GetMembers())
		{
			if (UMiscLibrary::IsCharacterAlive(character) && character != GetPlayerControllerOwner())
			{
				AAllyAIController* con = Cast<AAllyAIController>(character->GetController());

				if (con->IsTargetValid(cursorTarget))
				{
					con->SetPlayerControlledLocation(FVector::ZeroVector);
					con->SetTarget(cursorTarget);
				}
			}
		}
	}
}

void AMech_RPGPlayerController::AllyMove(int index)
{
	UGroup* group = GetPlayerControllerOwner()->GetGroup();
	if (group != nullptr)
	{
		AMech_RPGCharacter* character = group->GetMember(index);
		if (UMiscLibrary::IsCharacterAlive(character) && character != GetPlayerControllerOwner())
		{
			AAllyAIController* con = Cast<AAllyAIController>(character->GetController());
			static FHitResult Hit;

			GetHitResultUnderCursor(mWorldCollision, false, Hit);

			if (con != nullptr && Hit.bBlockingHit)
			{
				con->SetPlayerControlledLocation(Hit.ImpactPoint);
			}
		}
	}
}

void AMech_RPGPlayerController::AltPressed()
{
	altPressed = true;
}

void AMech_RPGPlayerController::AltReleased()
{
	altPressed = false;
}

void AMech_RPGPlayerController::CtrlPressed()
{
	ctrlPressed = true;
}

void AMech_RPGPlayerController::CtrlReleased()
{
	ctrlPressed = false;
}

void AMech_RPGPlayerController::ShiftPressed()
{
	shiftPressed = true;
}

void AMech_RPGPlayerController::ShiftReleased()
{
	shiftPressed = false;
}

AMech_RPGCharacter* AMech_RPGPlayerController::GetTarget()
{
	return target;
}

void AMech_RPGPlayerController::SetTarget(AMech_RPGCharacter* newVal)
{
	target = newVal;
}