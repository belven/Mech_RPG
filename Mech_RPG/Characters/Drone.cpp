// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "AllyAIController.h"
#include "Drone.h"

ADrone::ADrone() : Super()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> newMesh(TEXT("/Game/TopDown/Meshes/Drone.Drone"));

	if (newMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(newMesh.Object);
		//GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}

	AIControllerClass = AAllyAIController::StaticClass();
}

void ADrone::CreatePresetRole(ERole inRole, int32 grade, int32 quaility)
{
	Super::CreatePresetRole(ERole::Drone, grade, quaility);
}

void ADrone::OutOfCombat()
{
	Super::OutOfCombat();
}

void ADrone::SetDead(bool newVal)
{
	Super::SetDead(newVal);
}
