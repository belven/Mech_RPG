#pragma once
#include "Mech_RPG.h"
#include "Engine.h"
#include "Mech_RPGPlayerController.h"
#include "Mech_RPGCharacter.h"
#include "AI/Navigation/NavigationSystem.h"
#include "BaseAIController.h"
#include "AllyAIController.h"

AMech_RPGPlayerController::AMech_RPGPlayerController() {
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Hand;
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

		// Is our owner is still alive
		if (!GetOwner()->IsDead()) {

			// Check if we want to swap weapons
			if (swapWeapons) {
				GetOwner()->SwapWeapon();
				swapWeapons = false;
			}

			if (IsTargetValid(target) && bAttackTarget) {
				CurrentMouseCursor = EMouseCursor::Crosshairs;
				AttackTarget(DeltaTime);
			}
			else {
				target = GetTargetUnderCursor();

				if (IsTargetValid(target)) {
					CurrentMouseCursor = EMouseCursor::Crosshairs;

					if (bAttackTarget) {
						AttackTarget(DeltaTime);
					}
				}
				else {
					CurrentMouseCursor = EMouseCursor::Hand;
				}
			}

			if (!bAttackTarget && bMoveToMouseCursor) {
				OnAttackReleased();
				MoveToMouseCursor();
			}
		}
		else {
			PlayerDied();
		}
	}
}

void AMech_RPGPlayerController::AttackTarget(float DeltaTime) {
	AWeapon* weapon = owner->GetCurrentWeapon();

	if (weapon != NULL) {
		float dist = FVector::Dist(owner->GetActorLocation(), target->GetActorLocation());

		if (dist <= weapon->GetRange()) {
			if (weapon->CanFire()) {
				weapon->Fire(target, GetOwner());
			}
			StopMovement();
		}
		else if (GetWorld()->GetNavigationSystem()) {
			GetWorld()->GetNavigationSystem()->SimpleMoveToLocation(this, target->GetActorLocation());
		}
	}
}

void AMech_RPGPlayerController::SetupInputComponent() {
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
	InputComponent->BindAction("ActivateAbility", IE_Released, this, &AMech_RPGPlayerController::ActivateAbility);

	InputComponent->BindAction("Shift", IE_Pressed, this, &AMech_RPGPlayerController::ShiftPressed);
	InputComponent->BindAction("Shift", IE_Released, this, &AMech_RPGPlayerController::ShiftReleased);

	InputComponent->BindAction("Alt", IE_Pressed, this, &AMech_RPGPlayerController::AltPressed);
	InputComponent->BindAction("Alt", IE_Released, this, &AMech_RPGPlayerController::AltReleased);

	InputComponent->BindAction("Ctrl", IE_Pressed, this, &AMech_RPGPlayerController::CtrlPressed);
	InputComponent->BindAction("Ctrl", IE_Released, this, &AMech_RPGPlayerController::CtrlReleased);
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
			NavSys->SimpleMoveToLocation(this, DestLocation);
		}
	}
}

/**
 * Input handlers for SetDestination action.
 */
void AMech_RPGPlayerController::OnSetDestinationPressed() {
	bMoveToMouseCursor = true;
}

void AMech_RPGPlayerController::OnSetDestinationReleased() {
	bMoveToMouseCursor = false;
}

/**
 * Input handlers for Attack action.
 */
void AMech_RPGPlayerController::OnAttackPressed() {
	bAttackTarget = true;
}

void AMech_RPGPlayerController::OnAttackReleased() {
	bAttackTarget = false;
}

AMech_RPGCharacter* AMech_RPGPlayerController::GetTargetUnderCursor() {
	static FHitResult Hit;
	FCollisionQueryParams collision;
	collision.AddIgnoredActor(owner);

	Hit = GetHitFromCursor();

	if (Hit.bBlockingHit) {
		static AActor* targetFound;
		targetFound = Hit.GetActor();

		if (targetFound != NULL
			&& targetFound->GetClass()->IsChildOf(AMech_RPGCharacter::StaticClass())) {
			return Cast<AMech_RPGCharacter>(targetFound);
		}
	}
	return NULL;
}

bool AMech_RPGPlayerController::IsTargetValid(AMech_RPGCharacter* inTarget) {
	if (inTarget != NULL && !inTarget->IsDead()) {
		if (GetOwner()->GetCurrentWeapon()) {
			if (GetOwner()->GetCurrentWeapon()->Heals()) {
				return inTarget->CompareGroup(owner) && inTarget->GetHealth() < inTarget->GetMaxHealth();
			}
			else {
				return !inTarget->CompareGroup(owner);
			}
		}
		return false;
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
	if (IsOwnerValid() && GetOwner()->GetAbilities().Num() > 0) {
		if (!GetOwner()->GetAbilities()[0]->OnCooldown()) {
			GetOwner()->GetAbilities()[0]->Activate(GetOwner(), target);
		}
	}
}

void AMech_RPGPlayerController::PlayerDied() {
	for (int i = 0; i < GetOwner()->GetGroup()->GetMembers().Num(); i++) {
		AMech_RPGCharacter* character = GetOwner()->GetGroup()->GetMembers()[i];

		if (!character->IsDead()) {
			DemandSwapCharacter(i + 1);
		}
	}

	UnPossess();
	//GetOwner()->Destroy(true);
}

void AMech_RPGPlayerController::SwapCharacter() {
	ABaseAIController* con = Cast<ABaseAIController>(GetOwner()->GetDemandedController());
	AMech_RPGCharacter* other = con->GetOwner();

	GetOwner()->SetDemandedController(NULL);
	other->SetDemandedController(NULL);

	if (con != NULL) {
		con->Possess(GetOwner());
		Possess(other);
	}
}

bool AMech_RPGPlayerController::IsOwnerValid() {
	return GetOwner() != NULL && !GetOwner()->IsDead();
}

void AMech_RPGPlayerController::AllyAbility(int index) {

}

void AMech_RPGPlayerController::AllyAttack(int index) {
	static FHitResult Hit;
	UGroup* group = GetOwner()->GetGroup();

	if (group != NULL) {
		AMech_RPGCharacter* character = group->GetMember(index);
		if (!character->IsDead() && character != GetOwner()) {
			AAllyAIController* con = Cast<AAllyAIController>(character->GetController());

			AMech_RPGCharacter*	tempTarget = GetTargetUnderCursor();

			if (IsTargetValid(tempTarget)) {
				con->SetPlayerControlledLocation(FVector::ZeroVector);
				con->SetTarget(tempTarget);
			}
		}
	}
}

void AMech_RPGPlayerController::AllyMove(int index) {
	UGroup* group = GetOwner()->GetGroup();
	if (group != NULL) {
		AMech_RPGCharacter* character = group->GetMember(index);
		if (!character->IsDead() && character != GetOwner()) {
			AAllyAIController* con = Cast<AAllyAIController>(character->GetController());
			static FHitResult Hit;

			GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, Hit);

			if (Hit.bBlockingHit) {
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
