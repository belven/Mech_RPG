
#include "Mech_RPG.h"
#include "Mech_RPGCharacter.h"
#include "Snipe.h"
#include "Engine.h"
#include "Mech_RPGPlayerController.h"
#include "BaseAIController.h"

AMech_RPGCharacter::AMech_RPGCharacter() {
	static int32 ID = 0;
	SetID(ID++);

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

	//aoe = CreateDefaultSubobject<USphereComponent>(TEXT("AOE"));
	//aoe->AttachParent = RootComponent;
	//aoe->SetSphereRadius(1000);
	//aoe->SetWorldLocation(GetActorLocation());

	startingRole = GroupEnums::DPS;

}

void AMech_RPGCharacter::PossessedBy(AController* NewController) {
	Super::PossessedBy(NewController);
	AMech_RPGPlayerController* con = Cast<AMech_RPGPlayerController>(NewController);

	if (con) {
		con->SetOwner(this);
	}
	else {
		ABaseAIController* con = Cast<ABaseAIController>(NewController);
		if (con) {
			con->SetOwner(this);
			//aoe->OnComponentBeginOverlap.AddDynamic(con, &ABaseAIController::OnOverlapBegin);
			//aoe->OnComponentEndOverlap.AddDynamic(con, &ABaseAIController::OnOverlapEnd);
		}
	}

	Controller = NewController;
}

void AMech_RPGCharacter::Tick(float DeltaTime) {
	if (!isDead) {
		for (AWeapon* weapon : weapons) {
			if (weapon) {
				weapon->Tick(DeltaTime);
			}
		}

		if (GetHealth() < GetMaxHealth()) {
			health += healthRegen * DeltaTime;

			if (GetHealth() > GetMaxHealth()) {
				health = GetMaxHealth();
			}
		}
	}
}

void AMech_RPGCharacter::BeginPlay() {
	CreatePresetRole(startingRole);
	SetGroup(NULL);
	SetHealth(1000);
	SetMaxHealth(1000);
	SetHealthRegen(10.0);

	if (!GetGroup()) {
		SetGroup(UGroup::CreateGroup(startingGroupID));
		GetGroup()->AddMemeber(this);

		for (FConstPawnIterator iter = GetWorld()->GetPawnIterator(); iter; iter++) {
			APawn* pawn = iter->Get();
			if (pawn && pawn != this && pawn->GetDistanceTo(this) <= 500) {
				AMech_RPGCharacter* character = Cast<AMech_RPGCharacter>(pawn);
				character->SetGroup(GetGroup());
				GetGroup()->AddMemeber(character);
			}
		}
	}
}

void AMech_RPGCharacter::SwapWeapon() {
	if (weapons[0] == currentWeapon) {
		currentWeapon = weapons[1];
	}
	else {
		currentWeapon = weapons[0];
	}
}

void AMech_RPGCharacter::Hit(AMech_RPGCharacter* other, float damage) {
	health -= damage;

	if (health <= 0) {
		isDead = true;
		SetActorHiddenInGame(true);
		SetActorEnableCollision(false);
	}
}

void AMech_RPGCharacter::AddWeapon(AWeapon* newWeapon) {
	if (newWeapon) {
		weapons.Add(newWeapon);
	}
}

void AMech_RPGCharacter::CreatePresetRole(TEnumAsByte<GroupEnums::Role> role) {
	abilities.Empty();
	weapons.Empty();

	switch (role) {
	case GroupEnums::DPS:
		AddWeapon(AWeapon::CreatePresetWeapon(this, WeaponEnums::SMG));
		abilities.Add(USnipe::CreateAbility(15.0F));
		break;

	case GroupEnums::Healer:
		AddWeapon(AWeapon::CreatePresetWeapon(this, WeaponEnums::Bio_Repair));
		break;

	case GroupEnums::Tank:
		AddWeapon(AWeapon::CreatePresetWeapon(this, WeaponEnums::Shotgun));
		break;

	default:
		AddWeapon(AWeapon::CreatePresetWeapon(this, WeaponEnums::SMG));
		abilities.Add(USnipe::CreateAbility(5.0F));
		break;
	}
	currentWeapon = weapons[0];

}

float AMech_RPGCharacter::GetEnergy() {
	return energy;
}

float AMech_RPGCharacter::GetHealth() {
	return health;
}

void AMech_RPGCharacter::SetEnergy(float newVal) {
	energy = newVal;
}

void AMech_RPGCharacter::SetHealth(float newVal) {
	health = newVal;
}

TArray<AWeapon*> AMech_RPGCharacter::GetWeapons() {
	return weapons;
}

void AMech_RPGCharacter::SetWeapons(TArray<AWeapon*> newVal) {
	weapons = newVal;
}

bool AMech_RPGCharacter::IsDead() {
	return isDead;
}

void AMech_RPGCharacter::SetDead(bool newVal) {
	isDead = newVal;
}

AWeapon* AMech_RPGCharacter::GetCurrentWeapon() {
	return currentWeapon;
}

void AMech_RPGCharacter::SetCurrentWeapon(AWeapon* newVal) {
	currentWeapon = newVal;
}

UGroup* AMech_RPGCharacter::GetGroup() {
	return group;
}

void AMech_RPGCharacter::SetGroup(UGroup* newVal) {
	group = newVal;
}

int32 AMech_RPGCharacter::GetID() {
	return id;
}

void AMech_RPGCharacter::SetID(int32 newVal) {
	id = newVal;
}

bool AMech_RPGCharacter::CompareGroup(UGroup* inGroup) {
	return GetGroup() != NULL ? GetGroup()->Compare(inGroup) : true;
}

bool AMech_RPGCharacter::CompareGroup(AMech_RPGCharacter* inCharacter) {
	return inCharacter != NULL &&  inCharacter->GetGroup() != NULL ? CompareGroup(inCharacter->GetGroup()) : true;
}

AController* AMech_RPGCharacter::GetDemandedController() {
	return demandedController;
}

void AMech_RPGCharacter::SetDemandedController(AController* newVal) {
	demandedController = newVal;
}

TArray<UAbility*> AMech_RPGCharacter::GetAbilities() {
	return abilities;
}

void AMech_RPGCharacter::SetAbilities(TArray<UAbility*> newVal) {
	abilities = newVal;
}

USphereComponent* AMech_RPGCharacter::GetAOE() {
	return aoe;
}

void AMech_RPGCharacter::SetAOE(USphereComponent* newVal) {
	aoe = newVal;
}

float AMech_RPGCharacter::GetHealthRegen() {
	return healthRegen;
}

void AMech_RPGCharacter::SetHealthRegen(float newVal) {
	healthRegen = newVal;
}

float AMech_RPGCharacter::GetMaxHealth() {
	return maxHealth;
}

void AMech_RPGCharacter::SetMaxHealth(float newVal) {
	maxHealth = newVal;
}

bool AMech_RPGCharacter::CanAttack() {
	return canAttack == 0;
}

bool AMech_RPGCharacter::CanMove() {
	return canMove == 0;
}

float AMech_RPGCharacter::GetDamageModifier() {
	return damageModifier;
}

float AMech_RPGCharacter::GetDefenceModifier() {
	return defenceModifier;
}

void AMech_RPGCharacter::SetCanAttack(int32 newVal) {
	canAttack = newVal;
}

void AMech_RPGCharacter::SetCanMove(int32 newVal) {
	canMove = newVal;
}

void AMech_RPGCharacter::SetDamageModifier(float newVal) {
	damageModifier = newVal;
}

void AMech_RPGCharacter::SetDefenceModifier(float newVal) {
	defenceModifier = newVal;
}