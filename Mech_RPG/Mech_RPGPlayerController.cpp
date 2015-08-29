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
			SwapCharacter();
		}

		if (!GetOwner()->IsDead()){
			if (swapWeapons) {
				GetOwner()->SwapWeapon();
				swapWeapons = false;
			}

			GetTargetUnderCursor();

			if (IsTargetValid()){
				CurrentMouseCursor = EMouseCursor::Crosshairs;

				if (bAttackTarget){
					AttackTarget(DeltaTime);
				}
			}
			else {
				CurrentMouseCursor = EMouseCursor::Hand;
			}

			if (bMoveToMouseCursor)
			{
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
}


void AMech_RPGPlayerController::MoveToMouseCursor()
{
	FHitResult Hit;

	GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, Hit);

	if (Hit.bBlockingHit)
	{
		SetNewMoveDestination(Hit.ImpactPoint);
	}
}

void AMech_RPGPlayerController::SetNewMoveDestination(const FVector DestLocation)
{
	APawn* const Pawn = GetPawn();
	if (Pawn)
	{
		UNavigationSystem* const NavSys = GetWorld()->GetNavigationSystem();
		float const Distance = FVector::Dist(DestLocation, Pawn->GetActorLocation());

		if (NavSys)
		{
			NavSys->SimpleMoveToLocation(this, DestLocation);
		}
	}
}

void AMech_RPGPlayerController::OnSetDestinationPressed()
{
	bMoveToMouseCursor = true;
}

void AMech_RPGPlayerController::OnSetDestinationReleased()
{
	bMoveToMouseCursor = false;
}

void AMech_RPGPlayerController::OnAttackPressed()
{
	if (IsTargetValid()) {
		bAttackTarget = true;
	}
}

void AMech_RPGPlayerController::OnAttackReleased()
{
	target = NULL;
	bAttackTarget = false;
	CurrentMouseCursor = EMouseCursor::Hand;
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
			&& targetFound->GetClass()->IsChildOf(AMech_RPGCharacter::StaticClass()))
		{
			target = Cast<AMech_RPGCharacter>(targetFound);

			if (!IsTargetValid()){
				target = NULL;
				CurrentMouseCursor = EMouseCursor::Hand;
			}
		}
	}
}

bool AMech_RPGPlayerController::IsTargetValid(){
	return target && !target->IsDead() && !target->CompareGroup(owner);
}

void AMech_RPGPlayerController::DemandSwapCharacter(int index){
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
	DemandSwapCharacter(4);
}

void AMech_RPGPlayerController::CharacterOne(){
	DemandSwapCharacter(1);
}

void AMech_RPGPlayerController::CharacterThree(){
	DemandSwapCharacter(3);
}

void AMech_RPGPlayerController::CharacterTwo(){
	DemandSwapCharacter(2);
}

void AMech_RPGPlayerController::SwapWeapons(){
	if (IsOwnerValid()){
		swapWeapons = true;
	}
}

void AMech_RPGPlayerController::ActivateAbility(){
	if (IsOwnerValid() && GetOwner()->GetAbilities().Num() > 0) {
		if (!GetOwner()->GetAbilities()[0]->OnCooldown()) {
			GetOwner()->GetAbilities()[0]->Activate(GetOwner(), target);
		}
	}
}

void AMech_RPGPlayerController::PlayerDied(){
	for (int i = 0; i < GetOwner()->GetGroup()->GetMembers().Num(); i++){
		AMech_RPGCharacter* character = GetOwner()->GetGroup()->GetMembers()[i];

		if (!character->IsDead()){
			DemandSwapCharacter(i + 1);
		}
	}

	UnPossess();
	GetOwner()->Destroy(true);
}

void AMech_RPGPlayerController::SwapCharacter(){
	ABaseAIController* con = Cast<ABaseAIController>(GetOwner()->GetDemandedController());
	AMech_RPGCharacter* other = con->GetOwner();

	GetOwner()->SetDemandedController(NULL);
	other->SetDemandedController(NULL);

	if (con){
		con->Possess(GetOwner());
		Possess(other);
	}
}

bool AMech_RPGPlayerController::IsOwnerValid(){
	return GetOwner() && !GetOwner()->IsDead();
}