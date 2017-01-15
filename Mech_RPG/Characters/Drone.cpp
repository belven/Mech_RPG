// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "AllyAIController.h"
#include "Drone.h"

ADrone::ADrone() : Super() {
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> newMesh(TEXT("/Game/TopDown/Meshes/Drone.Drone"));

	if (newMesh.Succeeded()) {
		GetMesh()->SetSkeletalMesh(newMesh.Object);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}

	AIControllerClass = AAllyAIController::StaticClass();
}

void ADrone::CreatePresetRole(TEnumAsByte<GroupEnums::Role> inRole, int32 grade, int32 quaility) {
	Super::CreatePresetRole(inRole, grade, quaility);
	SetMaxHealth(GetMaxHealth() * 0.25F);
	GetAbilities().Empty();
	AddAbility(UTargetedHealthChange::CreateAbility(7, this, 0.25F, EAffectedTeam::Enemy));
	SetHealth(GetMaxHealth());
}

void ADrone::OutOfCombat() {
	// Prevent Re-spawn when out of combat
}
void ADrone::SetDead(bool newVal) {
	Super::SetDead(newVal);
	if (newVal) {
		//AMech_RPGCharacter::OutOfCombat();
		//SetHealth(GetMaxHealth());
	}
}
