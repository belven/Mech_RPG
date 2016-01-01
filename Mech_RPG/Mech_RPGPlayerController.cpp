#pragma once
#include "Mech_RPG.h"
#include "Engine.h"
#include "Mech_RPGPlayerController.h"
#include "AllyAIController.h"
#include "AI/Navigation/NavigationSystem.h"
//#include "Navigation/CrowdFollowingComponent.h"

AMech_RPGPlayerController::AMech_RPGPlayerController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	//: Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowingComponent"))) {
	bShowMouseCursor = true;
	bAttackTarget = false;
	DefaultMouseCursor = EMouseCursor::Hand;
	objectCollision.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);
	objectCollision.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);

	static ConstructorHelpers::FClassFinder<UUserWidget> characterPaneClass(TEXT("/Game/TopDown/Blueprints/UI/Character_Interface.Character_Interface_C"));

	if (characterPaneClass.Class != NULL) {
		WidgetTemplate = characterPaneClass.Class;
	}

}

void AMech_RPGPlayerController::BeginPlay() {
	if (WidgetTemplate != NULL) {
		characterPane = CreateWidget<UUserWidget>(this, WidgetTemplate);
		characterPane->AddToViewport();
		characterPane->SetVisibility(ESlateVisibility::Hidden);
		characterPane->SetPositionInViewport(FVector2D(1000, 200));

	}
}

/**
 * Begin PlayerController interface
 */
void AMech_RPGPlayerController::PlayerTick(float DeltaTime) {
	Super::PlayerTick(DeltaTime);

	//Do we have an owner
	if (GetOwner()) {

		// Is our owner demanding a character change
		if (GetOwner()->GetDemandedController() != NULL) {
			SwapCharacter();
		}

		if (panLeft || panRight) {
			FRotator rot = GetOwner()->CameraBoom->RelativeRotation;
			rot.Yaw += panLeft ? -1 : 1;
			GetOwner()->CameraBoom->SetRelativeRotation(rot);
		}

		// Is our owner is still alive
		if (!GetOwner()->IsDead()) {
			cursorTarget = GetTargetUnderCursor();

			if (cursorTarget != NULL && !cursorTarget->IsDead()) {
				CurrentMouseCursor = EMouseCursor::Crosshairs;
			}
			else {
				CurrentMouseCursor = EMouseCursor::Hand;
			}

			// Check if we want to swap weapons
			if (swapWeapons) {
				GetOwner()->SwapWeapon();
				swapWeapons = false;
			}

			if (IsTargetValid(target)) {
				AttackTarget(DeltaTime);
			}
			else if (IsTargetValid(cursorTarget) && bAttackTarget) {
				target = cursorTarget;
				AttackTarget(DeltaTime);
			}

		}
		else {
			PlayerDied();
		}
	}
}

void AMech_RPGPlayerController::MoveToActor(AActor* target) {
	if (GetOwner()->CanMove()) {
		GetWorld()->GetNavigationSystem()->SimpleMoveToActor(this, target);
	}
}

void AMech_RPGPlayerController::MoveToLocation(FVector location) {
	if (GetOwner()->CanMove()) {
		GetWorld()->GetNavigationSystem()->SimpleMoveToLocation(this, location);
	}
}

void AMech_RPGPlayerController::AttackTarget(float DeltaTime) {
	SetupCollision();

	GetWorld()->LineTraceSingleByObjectType(hit, GetOwner()->GetActorLocation(), target->GetActorLocation(), objectCollision, collision);

	bool targetTraced = hit.bBlockingHit && hit.GetActor() != NULL;
	
	GetOwner()->LookAt(target);

	// Are we targeting ourselves
	if (target == GetOwner()) {
		FireWeapon(NULL);
	}
	// Have we traced to another character or cover
	else if (targetTraced && (IsMechCharacter(hit.GetActor()) || IsCover(hit.GetActor()))) {
		FireWeapon(hit.GetActor());
	}
	// We've hit some scenery so move towards the target
	else if (GetWorld()->GetNavigationSystem()) {
		MoveToLocation(target->GetActorLocation());
	}
}

void AMech_RPGPlayerController::SetupCollision() {
	collision.IgnoreComponents.Empty();

	if (owner->GetGroup() != NULL && owner->GetGroup()->HasMemebers()) {
		for (AMech_RPGCharacter* member : owner->GetGroup()->GetMembers()) {
			if (member != target && member != cursorTarget) {
				collision.AddIgnoredActor(member);
			}
		}
	}
}

TArray<AMech_RPGCharacter*> AMech_RPGPlayerController::GetCharactersInRange(float range) {
	return UMiscLibrary::GetCharactersInRange(range, GetOwner());
}

void AMech_RPGPlayerController::FireWeapon(AActor* hit) {
	AWeapon* weapon = GetOwner()->GetCurrentWeapon();
	float distToTarget = FVector::Dist(GetOwner()->GetActorLocation(), target->GetActorLocation());

	// Are we in weapons range
	if (weapon != NULL && distToTarget <= weapon->GetRange()) {
		if (GetOwner()->CanAttack() && weapon->CanFire()) {

			bool isCover = hit != NULL ? IsCover(hit) : false;

			// Have we hit cover
			if (isCover) {
				float distToCover = FVector::Dist(GetOwner()->GetActorLocation(), hit->GetActorLocation());

				// Are we too far from the cover to avoid shooting it
				if (distToCover > 200) {
					weapon->Fire(Cast<ACover>(hit), GetOwner());
				}
				// Otherwise we can attack the target
				else {
					weapon->Fire(target, GetOwner());
				}
			}
			// Otherwise we've got a clear shot to the target
			else {
				weapon->Fire(target, GetOwner());
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

	InputComponent->BindAction("Attack", IE_Pressed, this, &AMech_RPGPlayerController::OnAttackPressed);
	InputComponent->BindAction("Attack", IE_Released, this, &AMech_RPGPlayerController::OnAttackReleased);

	InputComponent->BindAction("GroupAttack", IE_Pressed, this, &AMech_RPGPlayerController::GroupAttack);

	InputComponent->BindAction("One", IE_Released, this, &AMech_RPGPlayerController::CharacterOne);
	InputComponent->BindAction("Two", IE_Released, this, &AMech_RPGPlayerController::CharacterTwo);
	InputComponent->BindAction("Three", IE_Released, this, &AMech_RPGPlayerController::CharacterThree);
	InputComponent->BindAction("Four", IE_Released, this, &AMech_RPGPlayerController::CharacterFour);

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

	InputComponent->BindAction("ResetZoom", IE_Pressed, this, &AMech_RPGPlayerController::ResetZoom);
	InputComponent->BindAction("CharacterPane", IE_Pressed, this, &AMech_RPGPlayerController::OpenCharacterPane);
	//	InputComponent->BindAction("UpdateRotation", IE_Pressed, this, &AMech_RPGPlayerController::UpdateRotation);
}


void AMech_RPGPlayerController::OpenCharacterPane() {
	if (characterPaneOpen) {
		FInputModeGameAndUI data;
		SetInputMode(data);
		//SetPause(false);
		characterPaneOpen = false;
		characterPane->SetVisibility(ESlateVisibility::Hidden);
	}
	else {
		FInputModeUIOnly data;
		SetInputMode(data);
		//SetPause(true);
		characterPaneOpen = true;
		characterPane->SetVisibility(ESlateVisibility::Visible);
	}
}

/**
 * Navigate player to the current mouse cursor location.
 */
void AMech_RPGPlayerController::MoveToMouseCursor() {
	FHitResult Hit;

	GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, Hit);

	if (Hit.bBlockingHit) {
		SetNewMoveDestination(Hit.ImpactPoint);
	}
}

/**
 * Navigate player to the given world location.
 */
void AMech_RPGPlayerController::SetNewMoveDestination(const FVector DestLocation) {
	APawn* const Pawn = GetPawn();
	if (Pawn) {
		UNavigationSystem* const NavSys = GetWorld()->GetNavigationSystem();
		float const Distance = FVector::Dist(DestLocation, Pawn->GetActorLocation());

		if (NavSys) {
			MoveToLocation(DestLocation);
		}
	}
}

/**
 * Input handlers for SetDestination action.
 */
void AMech_RPGPlayerController::OnSetDestinationPressed() {
	bMoveToMouseCursor = true;

	target = NULL;
	OnAttackReleased();
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

void AMech_RPGPlayerController::OnAttackPressed() {
	bAttackTarget = true;
}

void AMech_RPGPlayerController::OnAttackReleased() {
	bAttackTarget = false;
}

AMech_RPGCharacter* AMech_RPGPlayerController::GetTargetUnderCursor() {
	FHitResult Hit;
	FCollisionQueryParams collision;
	collision.AddIgnoredActor(owner);

	Hit = GetHitFromCursor();

	if (Hit.bBlockingHit) {
		AActor* targetFound;
		targetFound = Hit.GetActor();

		if (targetFound != NULL	&& IsMechCharacter(targetFound)) {
			return Cast<AMech_RPGCharacter>(targetFound);
		}
	}
	return NULL;
}

bool AMech_RPGPlayerController::IsMechCharacter(AActor* character) {
	return character->GetClass()->IsChildOf(AMech_RPGCharacter::StaticClass());
}

bool AMech_RPGPlayerController::IsCover(AActor* character) {
	return character->GetClass()->IsChildOf(ACover::StaticClass());
}

bool AMech_RPGPlayerController::IsTargetValid(AMech_RPGCharacter* inTarget) {
	if (inTarget != NULL && !inTarget->IsDead()) {
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
	if (GetOwner() != NULL) {
		UGroup* group = GetOwner()->GetGroup();

		// Does the owner have a group and is there more than 1 other person
		if (group != NULL && group->GetMembers().Num() > 1) {

			// Does the character we want to swawp to exist
			if (group->GetMembers().Num() > index - 1) {
				AMech_RPGCharacter* character = group->GetMembers()[index - 1];

				// Are we swapping to ourselves
				if (GetOwner() != character && !character->IsDead()) {
					character->SetDemandedController(this);

					GetOwner()->SetDemandedController(character->GetController());
				}
			}
		}
	}
}

FHitResult AMech_RPGPlayerController::GetHitFromCursor() {
	static FHitResult Hit;
	GetHitResultUnderCursor(ECollisionChannel::ECC_Pawn, false, Hit);
	return Hit;
}

AMech_RPGCharacter* AMech_RPGPlayerController::GetOwner() {
	return owner;
}

void AMech_RPGPlayerController::SetOwner(AMech_RPGCharacter* newVal) {
	owner = newVal;
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

	if (IsOwnerValid()
		&& GetOwner()->HasAbilities()
		&& !GetOwner()->Channelling()
		&& GetOwner()->CanCast()) {

		SetupCollision();

		//We're targetting the ground so get selected location
		if (cursorTarget == NULL) {
			hit = GetHitFromCursor();
			location = hit.bBlockingHit ? hit.ImpactPoint : FVector::ZeroVector;
		}
		else {
			location = cursorTarget->GetActorLocation();
		}

		GetWorld()->LineTraceSingleByObjectType(hit, GetOwner()->GetActorLocation(), location, objectCollision, collision);

		//Only use an ability if we have LoS to our target/location
		if (hit.bBlockingHit) {
			// Have we hit something that isn't a mech
			if (hit.GetActor() == NULL || !UMiscLibrary::IsMechCharacter(hit.GetActor())) {
				MoveToLocation(location);
				return;
			}
		}

		for (UAbility* ability : GetOwner()->GetAbilities()) {
			if (ability != NULL && !ability->OnCooldown()) {
				ability->Activate(cursorTarget, location);

				if (ability->OnCooldown()) {
					GetOwner()->SetCurrentAbility(ability);
					StopMovement();
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

	UnPossess();
	//GetOwner()->Destroy(true);
}

void AMech_RPGPlayerController::SwapCharacter() {
	AAllyAIController* con = Cast<AAllyAIController>(GetOwner()->GetDemandedController());
	AMech_RPGCharacter* other = con->GetOwner();

	GetOwner()->SetDemandedController(NULL);
	other->SetDemandedController(NULL);

	if (con != NULL) {
		con->SetPlayerControlledLocation(FVector::ZeroVector);
		con->Possess(GetOwner());
		other->MirrorCamera(GetOwner());
		Possess(other);
	}
}

bool AMech_RPGPlayerController::IsOwnerValid() {
	return GetOwner() != NULL && !GetOwner()->IsDead();
}

void AMech_RPGPlayerController::AllyAbility(int index) {

}

void AMech_RPGPlayerController::AllyAttack(int index) {
	UGroup* group = GetOwner()->GetGroup();

	if (group != NULL) {
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

	if (group != NULL) {
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
	if (group != NULL) {
		AMech_RPGCharacter* character = group->GetMember(index);
		if (UMiscLibrary::IsCharacterAlive(character) && character != GetOwner()) {
			AAllyAIController* con = Cast<AAllyAIController>(character->GetController());
			static FHitResult Hit;

			GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, Hit);

			if (con != NULL && Hit.bBlockingHit) {
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