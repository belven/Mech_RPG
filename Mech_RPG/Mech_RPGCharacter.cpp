#pragma once
#include "Mech_RPG.h"
#include "Engine.h"
#include "Armour.h"
#include "BaseAIController.h"
#include "AllyAIController.h"
#include "Mech_RPGPlayerController.h"
#include "Navigation/CrowdFollowingComponent.h"

#define MIN(a,b) (a < b) ? (a) : (b)
#define MAX(a,b) (a > b) ? (a) : (b)

TArray<AMech_RPGCharacter*> AMech_RPGCharacter::characters;

AMech_RPGCharacter::AMech_RPGCharacter() {
	static int32 ID = 0;
	SetID(ID++);
	SetActorTickEnabled(true);

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->AttachTo(RootComponent);
	CameraBoom->bAbsoluteRotation = true; // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 1800.0f;
	CameraBoom->RelativeRotation = FRotator(-75.f, 0.f, 0.f);
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->AttachTo(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
	

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

	startingRole = GroupEnums::DPS;
	damageModifier = 1;
	defenceModifier = 0;
	movementModifier = 1.0;

	SetMaxHealth(2000);
	SetHealth(GetMaxHealth());

	speed = GetCharacterMovement()->MaxWalkSpeed;
	GetCharacterMovement()->bCanWalkOffLedges = false;
	channeling = false;
	team = TeamEnums::Paladins;
	characters.Add(this);
}

AMech_RPGCharacter::~AMech_RPGCharacter() {
	characters.Remove(this);
}

const TArray<AMech_RPGCharacter*>& AMech_RPGCharacter::GetCharacters() {
	return characters;
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
		}
	}

	Controller = NewController;
}

void AMech_RPGCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	if (!isDead) {
		GetCharacterMovement()->MaxWalkSpeed = speed * movementModifier;

		for (AWeapon* weapon : weapons) {
			if (weapon) {
				weapon->Tick(DeltaTime);
			}
		}

		if (GetHealth() < GetMaxHealth()) {
			float regen = !inCombat ? 200 : healthRegen;
			health += regen * DeltaTime;
		}

		if (GetHealth() > GetMaxHealth()) {
			health = GetMaxHealth();
		}
	}
}

void AMech_RPGCharacter::BeginPlay() {
	Super::BeginPlay();

	channeling = false;


	if (!UseLoadout) {
		CreatePresetRole(startingRole);
	}
	else {
		SetupWithLoadout();
	}

	if (weapons.Num() > 0) {
		currentWeapon = weapons[0];
	}

	if (abilities.Num() > 0) {
		SetCurrentAbility(abilities[0]);
	}

	/*while (settingUpGroups) {
		Sleep(500);
		}*/

	SetUpGroup();

	if (OnPostBeginPlay.IsBound()) {
		OnPostBeginPlay.Broadcast(this);
	}
}

TArray<UArmour*>& AMech_RPGCharacter::GetArmour() {
	return armour;
}

UArmour* AMech_RPGCharacter::GetArmourByPosition(TEnumAsByte<ArmourEnums::ArmourPosition> pos) {
	for (UArmour* armour : GetArmour()) {
		if (armour != NULL && armour->GetArmourPosition() == pos) return armour;
	}
	return NULL;
}

bool AMech_RPGCharacter::settingUpGroups = false;

void AMech_RPGCharacter::SetUpGroup() {
	if (group == NULL) {
		TArray<AMech_RPGCharacter*> charactersFound = UMiscLibrary::GetCharactersInRange(700, this);

		for (AMech_RPGCharacter* character : charactersFound) {
			if (character != this && character->team == team && character->GetGroup() != NULL) {
				SetGroup(character->GetGroup());
				GetGroup()->AddMemeber(this);
				break;
			}
		}
	}

	if (group == NULL) {
		SetGroup(UGroup::CreateGroup(team));
		group->AddMemeber(this);
	}

	ABaseAIController* con = Cast<ABaseAIController>(Controller);
	if (con) {
		group->OnMemberDamageEvent.AddUniqueDynamic(con, &ABaseAIController::GroupMemberDamaged);
	}

	group->OnMemberDamageEvent.AddUniqueDynamic(this, &AMech_RPGCharacter::SetInCombat);
}


void AMech_RPGCharacter::ApplyCrowdControl(TEnumAsByte<EffectEnums::CrowdControl> controlModifications, bool positive) {
	int amount = positive ? -1 : 1;

	switch (controlModifications) {
	case EffectEnums::Attack:
		canAttack += amount;
		break;
	case EffectEnums::Cast:
		canUseAbilities += amount;
		break;
	case EffectEnums::Damage:
		canBeDamaged += amount;
		break;
	case EffectEnums::Move:
		canMove += amount;
		break;
	}
}

void AMech_RPGCharacter::SwapWeapon() {
	if (weapons.Num() > 1) {
		if (weapons[0] == currentWeapon) {
			currentWeapon = weapons[1];
		}
		else {
			currentWeapon = weapons[0];
		}
	}
}

float AMech_RPGCharacter::GetTotalResistance(DamageEnums::DamageType damageType) {
	float totalResistance = 0;
	for (UArmour* armour : armour) {
		if (armour != NULL) totalResistance += armour->GetResistance(damageType);
	}

	return MAX(totalResistance, 1);
}

void AMech_RPGCharacter::ChangeHealth(FHealthChange damage) {
	if (GetGroup() != NULL) {
		GetGroup()->GroupMemberHit(damage.damager, this);
	}

	float resistance = (GetTotalResistance(damage.damageType) / 100);

	resistance *= (1 + GetDefenceModifier());
	resistance = MIN(0.99F, resistance);

	if (damage.healthChange < 0) {
		health -= damage.healthChange;
	}
	else if (canBeDamaged == 0) {
		health -= (damage.healthChange * (1 - resistance));
	}

	if (OnHealthChange.IsBound()) {
		OnHealthChange.Broadcast(damage);
	}

	if (health <= 0) {
		SetDead(true);
		SetActorHiddenInGame(true);
		SetActorEnableCollision(false);
	}
}

void AMech_RPGCharacter::SetInCombat(AMech_RPGCharacter* attacker, AMech_RPGCharacter* damagedMember) {
	inCombat = true;
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_OutOfCombat);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_OutOfCombat, this, &AMech_RPGCharacter::OutOfCombat, 3.0F);
}

void AMech_RPGCharacter::OutOfCombat() {
	inCombat = false;
}

void AMech_RPGCharacter::LookAt(AMech_RPGCharacter * other)
{
	FRotator rotation = GetActorRotation();

	FVector Direction = GetActorLocation() - other->GetActorLocation();
	Direction = FVector(Direction.X, Direction.Y, 0);

	rotation.Yaw = FRotationMatrix::MakeFromX(Direction).Rotator().Yaw + 180;

	SetActorRotation(rotation);
}

void AMech_RPGCharacter::AddWeapon(AWeapon* newWeapon) {
	if (newWeapon) {
		weapons.Add(newWeapon);
	}
}

void AMech_RPGCharacter::CreatePresetRole(TEnumAsByte<GroupEnums::Role> inRole) {
	float armourValue = 5;

	SetHealthRegen(10.0);

	float statModifier = 0;

	bool isPlayer = GetController() != NULL ? GetController()->GetClass()->IsChildOf(AMech_RPGPlayerController::StaticClass()) : false;
	bool isAlly = GetController() != NULL ? GetController()->GetClass()->IsChildOf(AAllyAIController::StaticClass()) : false;

	if (isAlly || isPlayer) {
		statModifier = GetModifierForDifficulty(GameEnums::Medium);
	}

	switch (inRole) {
	case GroupEnums::DPS:
		AddWeapon(AWeapon::CreatePresetWeapon(this, WeaponEnums::SMG));
		AddAbility(UChannelledAbility::CreateChannelledAbility(this, UGrenade::CreateAbility(7.0F, this, 300.0F), 2.0F, true));
		AddAbility(UTimedHealthChange::CreateTimedHealthChange(this, 10.0F));
		SetDefenceModifier(0.0F + statModifier);
		SetDamageModifier(1.0F + statModifier);
		armourValue = UArmour::GetDeafultValue(ArmourGrades::Light);
		SetMaxHealth(2000 * (1 + statModifier));
		break;

	case GroupEnums::Healer:
		AddWeapon(AWeapon::CreatePresetWeapon(this, WeaponEnums::Bio_Repair));
		AddAbility(UHeal::CreateAbility(15.0F, this, 1000.0F));
		AddAbility(UAoEHeal::CreateAbility(20.0F, this, 0.20F));
		SetDefenceModifier(0.0F + statModifier);
		SetDamageModifier(1.0F + statModifier);
		SetMovementModifier(1.0F + statModifier);
		armourValue = UArmour::GetDeafultValue(ArmourGrades::Medium);
		SetMaxHealth(2500 * (1 + statModifier));
		break;

	case GroupEnums::Tank:
		AddWeapon(AWeapon::CreatePresetWeapon(this, WeaponEnums::Shotgun));
		AddAbility(UTaunt::CreateAbility(5.0F, this));
		AddAbility(UDefenceBoost::CreateAbility(7.0F, this, 0.5F));
		SetDefenceModifier(0.0F + statModifier);
		SetDamageModifier(1.0F + statModifier);
		SetMovementModifier(1.0F + statModifier);
		armourValue = UArmour::GetDeafultValue(ArmourGrades::Heavy);
		SetMaxHealth(3000 * (1 + statModifier));
		break;

	case GroupEnums::Sniper:
		AddWeapon(AWeapon::CreatePresetWeapon(this, WeaponEnums::Sniper));
		AddAbility(UChannelledAbility::CreateChannelledAbility(this, USnipe::CreateAbility(15.0F, this), 2.5F, true, true));
		SetDefenceModifier(0.0F + statModifier);
		SetDamageModifier(1.0F + statModifier);
		armourValue = UArmour::GetDeafultValue(ArmourGrades::Light);
		SetMaxHealth(2000 * (1 + statModifier));
		break;

		//case GroupEnums::RPG:
		//	AddWeapon(AWeapon::CreatePresetWeapon(this, WeaponEnums::RPG));
		//	AddAbility(UChannelledAbility::CreateChannelledAbility(this, UOrbitalStrike::CreateAbility(10, this, 0.2F), 3));
		//	SetDefenceModifier(0 + statModifier);
		//	SetDamageModifier(1.25 + statModifier);
		//	break;

	case GroupEnums::Support:
		AddWeapon(AWeapon::CreatePresetWeapon(this, WeaponEnums::Shotgun));
		AddAbility(UDisable::CreateDisable(5.0F, this, 3.0F));
		AddAbility(UOrbitalStrike::CreateAbility(15.0F, this, 0.55F));
		SetDefenceModifier(0.0F + statModifier);
		SetDamageModifier(1.0F + statModifier);
		armourValue = UArmour::GetDeafultValue(ArmourGrades::Medium);
		SetMaxHealth(2500 * (1 + statModifier));
		break;

	default:
		CreatePresetRole(GroupEnums::DPS);
		break;
	}

	SetHealth(GetMaxHealth());
	armour.Empty();

	for (int i = 0; i < ArmourEnums::End; i++) {
		armour.Add(UArmour::CreateArmour(armourValue, armourValue, armourValue, (ArmourEnums::ArmourPosition)i));
	}
}

float AMech_RPGCharacter::GetModifierForDifficulty(TEnumAsByte<GameEnums::Difficulty> difficulty) {
	switch (difficulty) {
	case GameEnums::Easy:
		return 0.15;
	case GameEnums::Medium:
		return 0.10;
	case GameEnums::Hard:
		return 0.05;
	}
	return 0;
}

void AMech_RPGCharacter::SetupWithLoadout() {
	SetMaxHealth(startingLoadout.maxHealth);
	SetHealth(GetMaxHealth());
	SetDefenceModifier(startingLoadout.defenceModifier);
	SetDamageModifier(startingLoadout.damageModifier);
	SetCanMove(startingLoadout.canMove);
	SetCanAttack(startingLoadout.canAttack);
	SetCanBeDamaged(startingLoadout.canBeDamaged);
	SetHealthRegen(startingLoadout.healthRegen);
	team = startingLoadout.team;
	SetMovementModifier(startingLoadout.movementModifier);
	SetSpeed(startingLoadout.speed);
}

bool AMech_RPGCharacter::HasAbilities() {
	return GetAbilities().Num() > 0;
}


void AMech_RPGCharacter::SetChannelling(bool inChallenning) {
	channeling = inChallenning;
}

bool AMech_RPGCharacter::Channelling() {
	return channeling;
}

UAbility* AMech_RPGCharacter::GetCurrentAbility() {
	return currentAbility;
}

void AMech_RPGCharacter::SetCurrentAbility(UAbility* inAbility) {
	currentAbility = inAbility;
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

TArray<AWeapon*>& AMech_RPGCharacter::GetWeapons() {
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
	return inCharacter != NULL && inCharacter->GetGroup() != NULL ? CompareGroup(inCharacter->GetGroup()) : true;
}

AController* AMech_RPGCharacter::GetDemandedController() {
	return demandedController;
}

void AMech_RPGCharacter::SetDemandedController(AController* newVal) {
	demandedController = newVal;
}

TArray<UAbility*>& AMech_RPGCharacter::GetAbilities() {
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

bool AMech_RPGCharacter::CanCast() {
	return canUseAbilities == 0;
}

int32& AMech_RPGCharacter::GetCanAttack() {
	return canAttack;
}

int32& AMech_RPGCharacter::GetCanMove() {
	return canMove;
}

float AMech_RPGCharacter::GetDamageModifier() {
	return MAX(damageModifier, 0.01F);
}

float AMech_RPGCharacter::GetDefenceModifier() {
	return MIN(defenceModifier, 0.99F);
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

bool AMech_RPGCharacter::GetCanBeDamaged() {
	return canBeDamaged == 0;
}

float AMech_RPGCharacter::GetMovementModifier() {
	return movementModifier;
}

float AMech_RPGCharacter::GetSpeed() {
	return speed;
}

void AMech_RPGCharacter::SetCanBeDamaged(int32 newVal) {
	canBeDamaged = newVal;
}

void AMech_RPGCharacter::SetMovementModifier(float newVal) {
	movementModifier = newVal;
}

void AMech_RPGCharacter::SetSpeed(float newVal) {
	speed = newVal;
}

void AMech_RPGCharacter::AddAbility(UAbility* newAbility) {
	if (newAbility != NULL) {
		abilities.Add(newAbility);
	}
}