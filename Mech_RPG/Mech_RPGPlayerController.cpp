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

#define mCanSee(location) UMiscLibrary::CanSee(GetOwner()->GetWorld(), GetOwner()->GetActorLocation(), location)

AMech_RPGPlayerController::AMech_RPGPlayerController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	bShowMouseCursor = true;
	bAttackTarget = false;
	DefaultMouseCursor = EMouseCursor::Hand;
	objectCollision.AddObjectTypesToQuery(mWorldCollision);
	objectCollision.AddObjectTypesToQuery(mCharacterCollision);

	static ConstructorHelpers::FClassFinder<UUserWidget> characterPaneClass(TEXT("/Game/TopDown/Blueprints/UI/CharacterUI/Character_Pane.Character_Pane_C"));

	if (characterPaneClass.Class != nullptr) {
		WidgetTemplate = characterPaneClass.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> inventoryClass(TEXT("/Game/TopDown/Blueprints/UI/ItemsUI/Inventory.Inventory_C"));

	if (inventoryClass.Class != nullptr) {
		inventoryTemplate = inventoryClass.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> questListClass(TEXT("/Game/TopDown/Blueprints/UI/QuestList.QuestList_C"));

	if (questListClass.Class != nullptr) {
		questListTemplate = questListClass.Class;
	}

	SetActorTickEnabled(true);
	PrimaryActorTick.bCanEverTick = true;
	bEnableMouseOverEvents = true;

}

void AMech_RPGPlayerController::BeginPlay() {
	Super::BeginPlay();
	if (WidgetTemplate != nullptr) {
		characterPane = CreateWidget<UUserWidget>(this, WidgetTemplate);
		characterPane->AddToViewport();
		characterPane->SetVisibility(ESlateVisibility::Hidden);
	}

	if (inventoryTemplate != nullptr) {
		inventory = CreateWidget<UInventoryUI>(this, inventoryTemplate);
		inventory->AddToViewport();
		inventory->SetVisibility(ESlateVisibility::Hidden);
		inventory->SetPositionInViewport(FVector2D(600, 100));
	}

	if (questListTemplate != nullptr) {
		questList = CreateWidget<UQuestDisplayUI>(this, questListTemplate);
		questList->AddToViewport();
		questList->SetVisibility(ESlateVisibility::Visible);
		questList->SetPositionInViewport(FVector2D(1200, 100));
	}
}

/**
 * Begin PlayerController interface
 */
void AMech_RPGPlayerController::PlayerTick(float DeltaTime) {
	Super::PlayerTick(DeltaTime);
	//UE_LOG(LogTemp, Log, TEXT("PlayerTick"));

	//Do we have an owner
	if (GetOwner()) {

		// Is our owner demanding a character change
		if (GetOwner()->GetDemandedController() != nullptr) {
			SwapCharacter();
		}

		if (panLeft || panRight) {
			FRotator rot = GetOwner()->CameraBoom->RelativeRotation;
			rot.Yaw += panLeft ? -1 : 1;
			GetOwner()->CameraBoom->SetRelativeRotation(rot);
		}

		if (panUp || panDown) {
			FRotator rot = GetOwner()->CameraBoom->RelativeRotation;
			rot.Pitch += panUp ? -1 : 1;

			if (rot.Pitch < -85) {
				rot.Pitch = -85;
			}
			else if (rot.Pitch > 0) {
				rot.Pitch = 0;
			}

			GetOwner()->CameraBoom->SetRelativeRotation(rot);
		}

		// Is our owner is still alive
		if (!GetOwner()->IsDead()) {
			cursorTarget = GetTargetUnderCursor();

			if (cursorTarget != nullptr && !cursorTarget->IsDead()) {
				CurrentMouseCursor = EMouseCursor::Crosshairs;
			}
			else if (GetInteractableUnderCursor() != nullptr) {
				CurrentMouseCursor = EMouseCursor::GrabHand;
			}
			else {
				CurrentMouseCursor = EMouseCursor::Hand;
			}

			// Are we trying to interact with an Interactable
			if (lastAction == PlayerControllerEnums::Interactable) {
				if (mCanSee(lastTargetInteractable->GetActorLocation())
					&& GetOwner()->GetDistanceTo(lastTargetInteractable) <= interactionRange) {
					lastAction = PlayerControllerEnums::None;
					GetOwner()->Interact(lastTargetInteractable);
					inventory->GenerateInventory();
				}
				else {
					MoveToActor(lastTargetInteractable);
				}
			}
			// Are we trying to use an ability
			else if (lastAction == PlayerControllerEnums::Ability && lastCharacterTarget != nullptr) {
				if (mCanSee(lastCharacterTarget->GetActorLocation())) {
					ActivateAbility();
				}
				else {
					MoveToActor(lastCharacterTarget);
				}
			}
			// Are we trying to interact with a character
			else if (lastAction == PlayerControllerEnums::NPCInteract) {
				// Are we requesting a new target to attack			
				if (bAttackTarget && IsTargetValid(cursorTarget)) {
					target = cursorTarget;
					AttackTarget(DeltaTime);
				}
				// Is our current target valid
				else if (IsTargetValid(target)) {
					AttackTarget(DeltaTime);
				}
				// No valid target so stop firing
				else {
					GetOwner()->OnStopFiring.Broadcast();
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
		else {
			PlayerDied();
		}
	}
}

void AMech_RPGPlayerController::MoveToActor(AActor* target) {
	if (GetOwner()->CanMove()) {
		GetOwner()->OnStopFiring.Broadcast();
		GetWorld()->GetNavigationSystem()->SimpleMoveToActor(this, target);
	}
}

void AMech_RPGPlayerController::MoveToLocation(FVector location) {
	if (GetOwner()->CanMove()) {
		GetOwner()->OnStopFiring.Broadcast();
		GetWorld()->GetNavigationSystem()->SimpleMoveToLocation(this, location);
	}
}

void AMech_RPGPlayerController::AttackTarget(float DeltaTime) {
	// Are we targeting ourselves
	if (target == GetOwner()) {
		FireWeapon(nullptr);
		GetOwner()->LookAt(target);
	}
	// Have we traced to another character or cover
	else if (mCanSee(target->GetActorLocation())) {
		FireWeapon(target);
		GetOwner()->LookAt(target);
	}
	// We've hit some scenery so move towards the target
	else if (GetWorld()->GetNavigationSystem()) {
		MoveToLocation(target->GetActorLocation());
	}
}

void AMech_RPGPlayerController::SetupCollision() {
	collision.ClearIgnoredComponents();

	if (owner->GetGroup() != nullptr && owner->GetGroup()->HasMemebers()) {
		for (AMech_RPGCharacter* member : owner->GetGroup()->GetMembers()) {
			if (member != target && member != cursorTarget) {
				collision.AddIgnoredActor(member);
			}
		}
	}
}

void AMech_RPGPlayerController::AddQuest(UQuest * newQuest)
{
	if (questList != nullptr) {
		questList->GenerateQuests();
	}
}

void AMech_RPGPlayerController::PlayerItemPickup(AItem* item)
{
	inventory->GenerateInventory();
}

void AMech_RPGPlayerController::PlayerSwappedWeapons(AWeapon* oldWeapon, AWeapon* newWeapon)
{
	//characterPane->UpdateWeaponBar();
}

TArray<AMech_RPGCharacter*> AMech_RPGPlayerController::GetCharactersInRange(float range) {
	return UMiscLibrary::GetCharactersInRange(range, GetOwner());
}

void AMech_RPGPlayerController::FireWeapon(AActor* hit) {
	AWeapon* weapon = GetOwner()->GetCurrentWeapon();
	float distToTarget = FVector::Dist(GetOwner()->GetActorLocation(), target->GetActorLocation());

	// Are we in weapons range
	if (weapon != nullptr && distToTarget <= weapon->GetRange()) {
		if (GetOwner()->CanAttack() && weapon->CanFire()) {

			bool isCover = hit != nullptr ? IsCover(hit) : false;

			// Have we hit cover
			if (isCover) {
				float distToCover = FVector::Dist(GetOwner()->GetActorLocation(), hit->GetActorLocation());

				// Are we too far from the cover to avoid shooting it
				if (distToCover > 200) {
					weapon->Fire(Cast<ACover>(hit));
				}
				// Otherwise we can attack the target
				else {
					weapon->Fire(target);
				}
			}
			// Otherwise we've got a clear shot to the target
			else {
				weapon->Fire(target);
			}
		}

		owner->GetGroup()->GroupMemberHit(target, owner);
		StopMovement();
	}
	// We're out of range so move closer
	else if (GetWorld()->GetNavigationSystem()) {
		MoveToLocation(target->GetActorLocation());
	}
}

void AMech_RPGPlayerController::SetupInputComponent() {
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
	//	InputComponent->BindAction("UpdateRotation", IE_Pressed, this, &AMech_RPGPlayerController::UpdateRotation);
}

void AMech_RPGPlayerController::OpenCharacterPane() {
	if (characterPane->GetVisibility() == ESlateVisibility::Visible) {
		//FInputModeGameAndUI data;
		//SetInputMode(data);
		//SetPause(false);
		characterPaneOpen = false;
		characterPane->SetVisibility(ESlateVisibility::Hidden);
	}
	else {
		//FInputModeUIOnly data;
		//SetInputMode(data);
		//SetPause(true);
		characterPaneOpen = true;
		characterPane->SetVisibility(ESlateVisibility::Visible);
	}
}

void AMech_RPGPlayerController::OpenInventory()
{
	if (inventory->GetVisibility() == ESlateVisibility::Visible) {
		//FInputModeGameAndUI data;
		//SetInputMode(data);
		//SetPause(false);
		inventoryOpen = false;
		inventory->SetVisibility(ESlateVisibility::Hidden);
	}
	else {
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
void AMech_RPGPlayerController::MoveToMouseCursor() {
	FHitResult Hit;

	GetHitResultUnderCursor(mWorldCollision, false, Hit);

	//if (Hit.bBlockingHit) {
	SetNewMoveDestination(Hit.ImpactPoint);
	//}
}

/**
 * Navigate player to the given world location.
 */
void AMech_RPGPlayerController::SetNewMoveDestination(const FVector DestLocation) {
	UNavigationSystem* const NavSys = GetWorld()->GetNavigationSystem();

	if (UMiscLibrary::IsCharacterAlive(GetOwner()) && NavSys != nullptr) {
		MoveToLocation(DestLocation);
	}
}

/**
 * Input handlers for SetDestination action.
 */
void AMech_RPGPlayerController::OnSetDestinationPressed() {
	bMoveToMouseCursor = true;

	target = nullptr;
	lastCharacterTarget = nullptr;
	lastTargetInteractable = nullptr;
	lastUsedAbility = nullptr;
	lastAction = PlayerControllerEnums::None;
	OnRightClickReleased();
	MoveToMouseCursor();
}

void AMech_RPGPlayerController::OnSetDestinationReleased() {
	bMoveToMouseCursor = false;
}

void AMech_RPGPlayerController::ZoomIn() {
	GetOwner()->CameraBoom->TargetArmLength += 100;
}

void AMech_RPGPlayerController::ZoomOut() {
	GetOwner()->CameraBoom->TargetArmLength -= 100;
}

void AMech_RPGPlayerController::ResetZoom() {
	GetOwner()->CameraBoom->TargetArmLength = 1700;
	FRotator rot = GetOwner()->CameraBoom->RelativeRotation;
	rot.Yaw = GetOwner()->GetViewRotation().Yaw;
	rot.Pitch = -75.f;
	GetOwner()->CameraBoom->SetRelativeRotation(rot);
}

void AMech_RPGPlayerController::PanLeft()
{
	panLeft = true;
	FRotator rot = GetOwner()->CameraBoom->RelativeRotation;
	rot.Yaw -= 18;
	GetOwner()->CameraBoom->SetRelativeRotation(rot);
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

void AMech_RPGPlayerController::OnRightClickPressed() {
	if (GetInteractableUnderCursor() != nullptr) {
		lastAction = PlayerControllerEnums::Interactable;
		lastTargetInteractable = GetInteractableUnderCursor();
	}
	else {
		bAttackTarget = true;
		lastAction = PlayerControllerEnums::NPCInteract;
	}
}

void AMech_RPGPlayerController::OnRightClickReleased() {
	bAttackTarget = false;
}

AMech_RPGCharacter* AMech_RPGPlayerController::GetTargetUnderCursor() {
	FHitResult Hit;
	Hit = GetHitFromCursor();

	if (Hit.bBlockingHit) {
		AActor* targetFound = Hit.GetActor();

		if (targetFound != nullptr && IsMechCharacter(targetFound)) {
			return Cast<AMech_RPGCharacter>(targetFound);
		}
	}
	return nullptr;
}

AInteractable* AMech_RPGPlayerController::GetInteractableUnderCursor() {
	FHitResult Hit;
	Hit = GetHitFromCursor();

	if (Hit.bBlockingHit || (Hit = GetHitFromCursor(mItemCollision)).bBlockingHit) {
		AActor* targetFound = Hit.GetActor();

		if (targetFound != nullptr && IsInteractable(targetFound)) {
			return Cast<AInteractable>(targetFound);
		}
	}
	return nullptr;
}

bool AMech_RPGPlayerController::IsMechCharacter(AActor* character) {
	return character->GetClass()->IsChildOf(AMech_RPGCharacter::StaticClass());
}

bool AMech_RPGPlayerController::IsInteractable(AActor* character) {
	return character->GetClass()->IsChildOf(AInteractable::StaticClass());
}

bool AMech_RPGPlayerController::IsCover(AActor* character) {
	return character->GetClass()->IsChildOf(ACover::StaticClass());
}

bool AMech_RPGPlayerController::IsTargetValid(AMech_RPGCharacter* inTarget) {
	if (inTarget != nullptr && !inTarget->IsDead()) {
		if (GetOwner()->GetCurrentWeapon()) {
			if (GetOwner()->GetCurrentWeapon()->Heals()) {
				return inTarget->CompareGroup(owner);
			}
			else {
				return !inTarget->CompareGroup(owner);
			}
		}
	}
	return false;
}

void AMech_RPGPlayerController::DemandSwapCharacter(int index) {
	// Do we have an owner
	if (GetOwner() != nullptr) {
		UGroup* group = GetOwner()->GetGroup();

		// Does the owner have a group and is there more than 1 other person
		if (group != nullptr && group->GetMembers().Num() > 1) {

			// Does the character we want to swap to exist
			if (group->GetMembers().Num() > index - 1) {
				AMech_RPGCharacter* character = group->GetMembers()[index - 1];

				// Are we swapping to ourselves
				if (character != nullptr && GetOwner() != character && !character->IsDead()) {
					character->SetDemandedController(this);

					GetOwner()->SetDemandedController(character->GetController());
				}
			}
		}
	}
}

FHitResult AMech_RPGPlayerController::GetHitFromCursor(ECollisionChannel channel) {
	static FHitResult Hit;
	GetHitResultUnderCursor(channel, true, Hit);
	return Hit;
}

AMech_RPGCharacter* AMech_RPGPlayerController::GetOwner() {
	return owner;
}

void AMech_RPGPlayerController::SetOwner(AMech_RPGCharacter* newVal) {
	owner = newVal;
	inventory->SetOwner(owner);
	questList->SetCharacter(owner);
	questList->GenerateQuests();
	owner->OnQuestAdded.AddUniqueDynamic(this, &AMech_RPGPlayerController::AddQuest);
	owner->OnItemPickUpEvent.AddUniqueDynamic(this, &AMech_RPGPlayerController::PlayerItemPickup);
	inventory->GenerateInventory();

	for (UItemUI* item : inventory->GetSelectedItems()) {
		item->DeselectItem();
	}
	inventory->GetSelectedItems().Empty();

}

void AMech_RPGPlayerController::CharacterFive() {
	PerformAllyCommand(5);
}

void AMech_RPGPlayerController::CharacterFour() {
	PerformAllyCommand(4);
}

void AMech_RPGPlayerController::CharacterOne() {
	PerformAllyCommand(1);
}

void AMech_RPGPlayerController::CharacterThree() {
	PerformAllyCommand(3);
}

void AMech_RPGPlayerController::CharacterTwo() {
	PerformAllyCommand(2);
}

void AMech_RPGPlayerController::PerformAllyCommand(int index) {
	if (shiftPressed) {
		AllyMove(index);
	}
	else if (altPressed) {
		AllyAbility(index);
	}
	else if (ctrlPressed) {
		AllyAttack(index);
	}
	else {
		DemandSwapCharacter(index);
	}
}

void AMech_RPGPlayerController::SwapWeapons() {
	if (IsOwnerValid()) {
		swapWeapons = true;
	}
}

void AMech_RPGPlayerController::ActivateAbility() {
	FHitResult hit;
	FVector location;
	AMech_RPGCharacter* tempCharacter = cursorTarget != nullptr ? cursorTarget : lastCharacterTarget;

	if (IsOwnerValid()
		&& GetOwner()->HasAbilities()
		&& !GetOwner()->Channelling()
		&& GetOwner()->CanCast()) {

		SetupCollision();

		//We're targeting the ground so get selected location
		if (!UMiscLibrary::IsCharacterAlive(tempCharacter)) {
			hit = GetHitFromCursor();
			location = hit.ImpactPoint;
		}
		else if (UMiscLibrary::IsCharacterAlive(tempCharacter)) {
			location = tempCharacter->GetActorLocation();
			lastCharacterTarget = tempCharacter;
		}

		for (UAbility* ability : GetOwner()->GetAbilities()) {
			if (ability != nullptr && !ability->OnCooldown()) {

				if (ability->GetTagTrue(ability->needsTargetTag)) {
					//Only use an ability if we have LoS to our target/location
					if (!mCanSee(location)) {
						MoveToLocation(location);
						lastUsedAbility = ability;
						lastAction = PlayerControllerEnums::Ability;
						break;
					}
				}

				if (ability->Activate(tempCharacter, location)) {
					GetOwner()->SetCurrentAbility(ability);
					StopMovement();
					lastAction = PlayerControllerEnums::None;
					break;
				}
			}
		}
	}
}

void AMech_RPGPlayerController::PlayerDied() {
	for (int i = 0; i < GetOwner()->GetGroup()->GetMembers().Num(); i++) {
		AMech_RPGCharacter* character = GetOwner()->GetGroup()->GetMembers()[i];

		if (UMiscLibrary::IsCharacterAlive(character)) {
			DemandSwapCharacter(i + 1);
			break;
		}
	}
}

void AMech_RPGPlayerController::SwapCharacter() {
	AAllyAIController* aiCon = Cast<AAllyAIController>(GetOwner()->GetDemandedController());
	AMech_RPGCharacter* aiOwner = aiCon->GetOwner();
	AMech_RPGCharacter* aiTarget = aiCon->GetTarget();

	GetOwner()->SetDemandedController(nullptr);
	aiOwner->SetDemandedController(nullptr);

	if (aiCon != nullptr) {
		aiCon->SetTarget(GetTarget());
		aiCon->SetPlayerControlledLocation(FVector::ZeroVector);
		aiCon->Possess(GetOwner());
		aiOwner->MirrorCamera(GetOwner());
		Possess(aiOwner);
	}
}

bool AMech_RPGPlayerController::IsOwnerValid() {
	return GetOwner() != nullptr && !GetOwner()->IsDead();
}

void AMech_RPGPlayerController::AllyAbility(int index) {

}

void AMech_RPGPlayerController::AllyAttack(int index) {
	UGroup* group = GetOwner()->GetGroup();

	if (group != nullptr) {
		AMech_RPGCharacter* character = group->GetMember(index);
		if (UMiscLibrary::IsCharacterAlive(character) && character != GetOwner()) {
			AAllyAIController* con = Cast<AAllyAIController>(character->GetController());

			if (con->IsTargetValid(cursorTarget)) {
				con->SetPlayerControlledLocation(FVector::ZeroVector);
				con->SetTarget(cursorTarget);
			}
		}
	}
}

void AMech_RPGPlayerController::GroupAttack() {
	UGroup* group = GetOwner()->GetGroup();

	if (group != nullptr) {
		for (AMech_RPGCharacter* character : group->GetMembers()) {
			if (UMiscLibrary::IsCharacterAlive(character) && character != GetOwner()) {
				AAllyAIController* con = Cast<AAllyAIController>(character->GetController());

				if (con->IsTargetValid(cursorTarget)) {
					con->SetPlayerControlledLocation(FVector::ZeroVector);
					con->SetTarget(cursorTarget);
				}
			}
		}
	}
}

void AMech_RPGPlayerController::AllyMove(int index) {
	UGroup* group = GetOwner()->GetGroup();
	if (group != nullptr) {
		AMech_RPGCharacter* character = group->GetMember(index);
		if (UMiscLibrary::IsCharacterAlive(character) && character != GetOwner()) {
			AAllyAIController* con = Cast<AAllyAIController>(character->GetController());
			static FHitResult Hit;

			GetHitResultUnderCursor(mWorldCollision, false, Hit);

			if (con != nullptr && Hit.bBlockingHit) {
				con->SetPlayerControlledLocation(Hit.ImpactPoint);
			}
		}
	}
}

void AMech_RPGPlayerController::AltPressed() {
	altPressed = true;
}

void AMech_RPGPlayerController::AltReleased() {
	altPressed = false;
}

void AMech_RPGPlayerController::CtrlPressed() {
	ctrlPressed = true;
}

void AMech_RPGPlayerController::CtrlReleased() {
	ctrlPressed = false;
}

void AMech_RPGPlayerController::ShiftPressed() {
	shiftPressed = true;
}

void AMech_RPGPlayerController::ShiftReleased() {
	shiftPressed = false;
}

AMech_RPGCharacter* AMech_RPGPlayerController::GetTarget() {
	return target;
}

void AMech_RPGPlayerController::SetTarget(AMech_RPGCharacter* newVal) {
	target = newVal;
}