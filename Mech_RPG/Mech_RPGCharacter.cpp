// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "Mech_RPG.h"
#include "Mech_RPGCharacter.h"
#include "Engine.h"
#include "Mech_RPGPlayerController.h"

AMech_RPGCharacter::AMech_RPGCharacter()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->AttachTo(RootComponent);
	CameraBoom->bAbsoluteRotation = true; // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 900.f;
	CameraBoom->RelativeRotation = FRotator(-60.f, 0.f, 0.f);
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->AttachTo(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	static int ID = 0;

	SetID(ID++);

	aoeDecetion = CreateDefaultSubobject<USphereComponent>(TEXT("AoeSphere"));
	aoeDecetion->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	aoeDecetion->SetSphereRadius(500);
	aoeDecetion->SetWorldLocation(this->GetActorLocation());
}

void AMech_RPGCharacter::PossessedBy(AController* NewController) {
	AMech_RPGPlayerController* con = Cast<AMech_RPGPlayerController>(NewController);
	if (con){
		con->SetOwner(this);
	}
}

void AMech_RPGCharacter::BeginPlay(){
	static int32 ID = 0;

	SetHealth(10000);
	SetGroup(NULL);

	aoeDecetion->SetWorldLocation(this->GetActorLocation());

	weapons = *new TArray<AWeapon*>();
	AddWeapon(currentWeapon = AWeapon::CreateWeapon(this, 10, 500, 0.5));
	
	if (!GetGroup()) {
		TArray<AActor*> OutOverlappingActors;

		SetGroup(UGroup::CreateGroup(ID, *new TArray<AMech_RPGCharacter*>()));
		GetGroup()->AddMemeber(this);

		aoeDecetion->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		
		aoeDecetion->GetOverlappingActors(OutOverlappingActors, GetClass());

		aoeDecetion->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		
		if (OutOverlappingActors.Num() > 0){
			for (AActor* actor : OutOverlappingActors){
				if (actor != this) {
					AMech_RPGCharacter* character = Cast<AMech_RPGCharacter>(actor);
					character->SetGroup(GetGroup());
					GetGroup()->AddMemeber(character);
				}
			}
		}

		ID++;
	}
}

float AMech_RPGCharacter::GetEnergy(){
	return energy;
}


float AMech_RPGCharacter::GetHealth(){
	return health;
}


void AMech_RPGCharacter::SetEnergy(float newVal){
	energy = newVal;
}


void AMech_RPGCharacter::SetHealth(float newVal){
	health = newVal;
}


void AMech_RPGCharacter::Hit(AMech_RPGCharacter* other, float damage){
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, "Damage taken: " + FString::SanitizeFloat(damage));
	health -= damage;

	if (health <= 0)
	{
		isDead = true;
	}
}

void AMech_RPGCharacter::AddWeapon(AWeapon* newWeapon){
	if (newWeapon){
		weapons.Add(newWeapon);
	}
}

TArray<AWeapon*> AMech_RPGCharacter::GetWeapons(){
	return weapons;
}

void AMech_RPGCharacter::SetWeapons(TArray<AWeapon*> newVal){
	weapons = newVal;
}

bool AMech_RPGCharacter::IsDead(){
	return isDead;
}

void AMech_RPGCharacter::SetDead(bool newVal){
	isDead = newVal;
}

AWeapon* AMech_RPGCharacter::GetCurrentWeapon(){
	return currentWeapon;
}

void AMech_RPGCharacter::SetCurrentWeapon(AWeapon* newVal){
	currentWeapon = newVal;
}

UGroup* AMech_RPGCharacter::GetGroup(){
	return group;
}

void AMech_RPGCharacter::SetGroup(UGroup* newVal){
	group = newVal;
}

int32 AMech_RPGCharacter::GetID(){
	return id;
}

void AMech_RPGCharacter::SetID(int32 newVal){
	id = newVal;
}


bool AMech_RPGCharacter::CompareGroup(UGroup* inGroup){
	return GetGroup()->Compare(inGroup);
}


bool AMech_RPGCharacter::CompareGroup(AMech_RPGCharacter* inCharacter){
	return CompareGroup(inCharacter->GetGroup());
}