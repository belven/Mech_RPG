#pragma once
#include "Mech_RPG.h"
#include "Engine.h"
#include "Armour.h"
#include "BaseAIController.h"
#include "AllyAIController.h"
#include "Blueprint/UserWidget.h"
#include "Mech_RPGPlayerController.h"
#include "FloatingTextUI.h"
#include "Group.h"

#define mCreatePresetWeapon(type, grade, quailty) AWeapon::CreatePresetWeapon(this, type, grade, quailty)
#define mCreatePresetAbility(type) UAbility::CreatePresetAbility(this,type)
#define mCreateChannelledAbility(ability, Duration, loc, trace) UChannelledAbility::CreateChannelledAbility(this, ability, Duration, loc, trace)
#define mCreatePresetRole(role) AMech_RPGCharacter::CreatePresetRole(role)

TArray<AMech_RPGCharacter*> AMech_RPGCharacter::characters;
bool AMech_RPGCharacter::settingUpGroups = false;

AMech_RPGCharacter::AMech_RPGCharacter() {
	static int32 ID = 0;
	SetID(ID++);
	SetActorTickEnabled(true);

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->AttachTo(RootComponent);
	CameraBoom->bAbsoluteRotation = true; // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 1800.0f;
	CameraBoom->RelativeRotation = FRotator(-85.f, 0.f, 0.f);
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->AttachTo(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm


	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

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
	healthChangeModifier = 1;
	defenceModifier = 0;
	movementModifier = 1.0;

	SetMaxHealth(2000);
	SetHealth(GetMaxHealth());

	speed = GetCharacterMovement()->MaxWalkSpeed;
	GetCharacterMovement()->bCanWalkOffLedges = false;
	channeling = false;
	team = TeamEnums::Paladins;
	characters.Add(this);

	static ConstructorHelpers::FClassFinder<UFloatingStats_BP> statsWidget(TEXT("/Game/TopDown/Blueprints/UI/FloatingStats"));

	if (statsWidget.Succeeded()) {
		widgetClass = statsWidget.Class;
		stats = CreateDefaultSubobject<UWidgetComponent>(TEXT("Floating Stats Component"));
		stats->AttachTo(GetRootComponent());
	}

	static ConstructorHelpers::FClassFinder<UFloatingTextUI> floatingTextWidget(TEXT("/Game/TopDown/Blueprints/UI/Floating_Text"));

	if (floatingTextWidget.Succeeded()) {
		floatingTextClass = floatingTextWidget.Class;
	}
	
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> newMesh(TEXT("/Game/TopDown/Meshes/Mecha_2.Mecha_2"));

	if (newMesh.Succeeded()) {
		GetMesh()->SetSkeletalMesh(newMesh.Object);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}

	AIControllerClass = ABaseAIController::StaticClass();


}

AMech_RPGCharacter::~AMech_RPGCharacter() {
	characters.Remove(this);
	abilities.Empty();
	weapons.Empty();
	armour.Empty();

	//if (stats != nullptr) {
	//	delete stats;
	//}
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
			float regen = !inCombat ? GetMaxHealth() * 0.15 : healthRegen;
			health += regen * DeltaTime;
		}

		if (GetHealth() > GetMaxHealth()) {
			health = GetMaxHealth();
		}

		if (stats->GetUserWidgetObject() != nullptr) {
			UFloatingStats_BP* statsBP = Cast<UFloatingStats_BP>(stats->GetUserWidgetObject());

			statsBP->Tick(DeltaTime);
			statsBP->SetOwner(this);
		}
	}
}

void AMech_RPGCharacter::BeginPlay() {
	Super::BeginPlay();
	if (IsPendingKill()) {
		return;
	}

	Reset();


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

	SetUpGroup();

	if (OnPostBeginPlay.IsBound()) {
		OnPostBeginPlay.Broadcast(this);
	}

	if (widgetClass != nullptr) {
		UFloatingStats_BP* widget = CreateWidget<UFloatingStats_BP>(GetWorld(), widgetClass);
		widget->SetOwner(this);
		stats->SetWidget(widget);

		FTransform trans;
		trans.SetLocation(FVector(-0.000035, 7.999790, 111.999756));
		trans.SetScale3D(FVector(0.25, 0.75, 1));
		stats->SetRelativeTransform(trans);
		stats->AttachTo(GetRootComponent());
		stats->SetDrawSize(FVector2D(100, 50));
	}

	if (inventory == nullptr) {
		inventory = NewObject<UInventory>(UInventory::StaticClass());
		if (inventory != nullptr) {
			int invSize = 20;
			inventory->SetMaxItemCount(invSize);
			inventory->AddItem(AItem::CreateItem(GetWorld(), this, "Item 1", 3, 0, 0, 5));
			inventory->AddItem(AItem::CreateItem(GetWorld(), this, "Item 1", 3, 0, 0, 5));
			inventory->AddItem(AItem::CreateItem(GetWorld(), this, "Item 2", 4, 0, 0, 2));
			inventory->AddItem(AItem::CreateItem(GetWorld(), this, "Item 3", 0, 0, 0, 2));
			inventory->AddItem(AItem::CreateItem(GetWorld(), this, "Item 4", 3, 0, 0, 1));
		}
	}
}

UArmour* AMech_RPGCharacter::GetArmourByPosition(TEnumAsByte<ArmourEnums::ArmourPosition> pos) {
	for (UArmour* armour : GetArmour()) {
		if (armour != nullptr && armour->GetArmourPosition() == pos) return armour;
	}
	return nullptr;
}

void AMech_RPGCharacter::SetUpGroup() {
	if (group == nullptr) {
		TArray<AMech_RPGCharacter*> charactersFound = UMiscLibrary::GetCharactersInRange(700, this);

		for (AMech_RPGCharacter* character : charactersFound) {
			if (character != this && character->team == team && character->GetGroup() != nullptr) {
				SetGroup(character->GetGroup());
				break;
			}
		}
	}

	if (group == nullptr) {
		SetGroup(UGroup::CreateGroup(team));
	}

	ABaseAIController* con = Cast<ABaseAIController>(Controller);
	if (con) {
		group->OnMemberDamageEvent.AddUniqueDynamic(con, &ABaseAIController::GroupMemberDamaged);
	}

	group->OnMemberDamageEvent.AddUniqueDynamic(this, &AMech_RPGCharacter::SetInCombat);
}


void AMech_RPGCharacter::ApplyCrowdControl(TEnumAsByte<EffectEnums::CrowdControl> controlModifications, bool remove) {
	int amount = remove ? -1 : 1;

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
			if (OnSwappedWeapons.IsBound()) {
				OnSwappedWeapons.Broadcast(currentWeapon, weapons[1]);
			}
			currentWeapon = weapons[1];
		}
		else {
			if (OnSwappedWeapons.IsBound()) {
				OnSwappedWeapons.Broadcast(currentWeapon, weapons[0]);
			}
			currentWeapon = weapons[0];
		}
	}
}

float AMech_RPGCharacter::GetTotalResistance(DamageEnums::DamageType damageType) {
	float totalResistance = 0;
	for (UArmour* armour : armour) {
		if (armour != nullptr) totalResistance += armour->GetResistance(damageType);
	}

	return MAX(totalResistance, 1);
}

void AMech_RPGCharacter::ChangeHealth(FHealthChange healthChange) {
	if (GetGroup() != nullptr && !CompareGroup(healthChange.damager)) {
		GetGroup()->GroupMemberHit(healthChange.damager, this);
	}

	if (OnPreHealthChange.IsBound()) {
		OnPreHealthChange.Broadcast(healthChange);
	}

	float resistance = (GetTotalResistance(healthChange.damageType) / 100);

	resistance *= (1 + GetDefenceModifier());
	resistance = MIN(0.99F, resistance);

	if (healthChange.heals) {
		health += healthChange.healthChange;
	}
	else if (canBeDamaged == 0) {
		health -= (healthChange.healthChange *= (1 - resistance));
	}

	if (OnPostHealthChange.IsBound()) {
		OnPostHealthChange.Broadcast(healthChange);
	}

	UFloatingTextUI::CreateFloatingText(floatingTextClass, healthChange);

	if (health <= 0) {
		health = 0;
		SetDead(true);
		SetActorHiddenInGame(true);
		OnStopFiring.Broadcast();
		healthChange.damager->OnEnemyKilled.Broadcast(this);
		healthChange.damager->GetGroup()->OnGroupEnemyKilled.Broadcast(this);
	}
}

void AMech_RPGCharacter::SetActorHiddenInGame(bool bNewHidden)
{
	Super::SetActorHiddenInGame(bNewHidden);
	SetActorEnableCollision(!bNewHidden);

	for (AWeapon* weapon : weapons) {
		weapon->SetActorHiddenInGame(bNewHidden);
	}
}

void AMech_RPGCharacter::SetInCombat(AMech_RPGCharacter* attacker, AMech_RPGCharacter* damagedMember) {
	inCombat = true;
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_OutOfCombat);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_OutOfCombat, this, &AMech_RPGCharacter::OutOfCombat, 3.0F);
}

void AMech_RPGCharacter::Reset()
{
	weapons.Empty();
	abilities.Empty();
	armour.Empty();
	SetHealth(GetMaxHealth());

	channeling = false;
	SetDead(false);
	OutOfCombat();
	canAttack = 0;
	canMove = 0;
	canBeDamaged = 0;
}

void AMech_RPGCharacter::ResetInvunrelbility()
{
	ApplyCrowdControl(EffectEnums::Damage, true);
}

UInventory * AMech_RPGCharacter::GetInventory()
{
	return inventory;
}

void AMech_RPGCharacter::OutOfCombat() {
	inCombat = false;
	OnStopFiring.Broadcast();

	if (OnOutOfCombat.IsBound()) OnOutOfCombat.Broadcast();

	if (IsDead() && GetGroup()->GetPlayer() != nullptr) {
		SetDead(false);
		SetActorHiddenInGame(false);
		SetActorEnableCollision(true);
		SetHealth(GetMaxHealth() * 0.2);
		ApplyCrowdControl(EffectEnums::Damage, false);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_Invunrelbility, this, &AMech_RPGCharacter::ResetInvunrelbility, 3.0F);
	}
}

void AMech_RPGCharacter::LookAt(AMech_RPGCharacter * other)
{
	FRotator rotation = GetActorRotation();

	FVector Direction = GetActorLocation() - other->GetActorLocation();
	Direction = FVector(Direction.X, Direction.Y, 0);

	rotation.Yaw = FRotationMatrix::MakeFromX(Direction).Rotator().Yaw + 180;

	SetActorRotation(rotation);
}

void AMech_RPGCharacter::CreatePresetRole(TEnumAsByte<GroupEnums::Role> inRole, int32 grade, int32 quaility) {
	static float armourValue = 5;
	static float statModifier = 0;
	static float lowHealth = 2000;
	static float mediumHealth = lowHealth * 1.25;
	static float highHealth = mediumHealth * 1.25;
	startingRole = inRole;

	Reset();

	bool isPlayer = mIsChildOf(GetController(), AMech_RPGPlayerController::StaticClass());
	bool isAlly = mIsChildOf(GetController(), AAllyAIController::StaticClass());

	if (isAlly || isPlayer) {
		statModifier = GetModifierForDifficulty(UMiscLibrary::GetDifficulty());
	}

	switch (inRole) {
	case GroupEnums::DPS:
		AddWeapon(mCreatePresetWeapon(WeaponEnums::SMG, grade, quaility));
		AddAbility(UAbility::CreateChannelledPresetAbility(this, AbilityEnums::Grenade, 1.0F, true, false));
		AddAbility(UTimedHealthChange::CreateTimedHealthChange(this, 10.0F, 200.0F));
		SetDefenceModifier(0.0F + statModifier);
		SetHealthChangeModifier(1.0F + statModifier);
		armourValue = UArmour::GetDeafultValue(ArmourGrades::Light);
		SetMaxHealth(lowHealth * (1 + statModifier));
		break;

	case GroupEnums::Healer:
		AddWeapon(mCreatePresetWeapon(WeaponEnums::Bio_Repair, grade, quaility));
		AddAbility(mCreatePresetAbility(AbilityEnums::Heal));
		AddAbility(mCreatePresetAbility(AbilityEnums::AoEHeal));
		SetDefenceModifier(0.0F + statModifier);
		SetHealthChangeModifier(1.0F + statModifier);
		SetMovementModifier(1.0F + statModifier);
		armourValue = UArmour::GetDeafultValue(ArmourGrades::Light);
		SetMaxHealth(lowHealth * (1 + statModifier));
		break;

	case GroupEnums::Tank:
		AddWeapon(mCreatePresetWeapon(WeaponEnums::Shotgun, grade, quaility));
		AddAbility(mCreatePresetAbility(AbilityEnums::Taunt));
		AddAbility(mCreatePresetAbility(AbilityEnums::Stun));
		SetDefenceModifier(0.0F + statModifier);
		SetHealthChangeModifier(1.0F + statModifier);
		SetMovementModifier(1.0F + statModifier);
		armourValue = UArmour::GetDeafultValue(ArmourGrades::Heavy);
		SetMaxHealth(highHealth * (1 + statModifier));
		break;

	case GroupEnums::Sniper:
		AddWeapon(mCreatePresetWeapon(WeaponEnums::Sniper, grade, quaility));
		AddAbility(UAbility::CreateChannelledPresetAbility(this, AbilityEnums::Snipe, 2.5F, false, false));
		AddAbility(mCreatePresetAbility(AbilityEnums::CritBoost));
		SetDefenceModifier(0.0F + statModifier);
		SetHealthChangeModifier(1.0F + statModifier);
		armourValue = UArmour::GetDeafultValue(ArmourGrades::Light);
		SetMaxHealth(lowHealth * (1 + statModifier));
		break;

	case GroupEnums::Support:
		AddWeapon(mCreatePresetWeapon(WeaponEnums::Shotgun, grade, quaility));
		AddAbility(USummonDamageDrone::CreateAbility(20, this));
		AddAbility(UShield::CreateShield(20, this, 0.35F));
		AddAbility(mCreatePresetAbility(AbilityEnums::Disable));
		SetDefenceModifier(0.0F + statModifier);
		SetHealthChangeModifier(1.0F + statModifier);
		armourValue = UArmour::GetDeafultValue(ArmourGrades::Medium);
		SetMaxHealth(mediumHealth * (1 + statModifier));
		break;

	default:
		CreatePresetRole(GroupEnums::DPS);
		break;
	}

	SetHealth(GetMaxHealth());

	if (weapons.Num() > 0) {
		currentWeapon = weapons[0];
	}

	for (int i = 0; i < ArmourEnums::End; i++) {
		armour.Add(UArmour::CreateArmour(armourValue, armourValue, armourValue, (ArmourEnums::ArmourPosition)i));
	}
}

float AMech_RPGCharacter::GetModifierForDifficulty(TEnumAsByte<GameEnums::Difficulty> difficulty) {
	switch (difficulty) {
	case GameEnums::Easy:
		return 0.20;
	case GameEnums::Medium:
		return 0.10;
	case GameEnums::Hard:
		return 0.0;
	}
	return 0;
}

void AMech_RPGCharacter::SetupWithLoadout() {
	SetMaxHealth(startingLoadout.maxHealth);
	SetHealth(GetMaxHealth());
	SetDefenceModifier(startingLoadout.defenceModifier);
	SetHealthChangeModifier(startingLoadout.damageModifier);
	SetCanMove(startingLoadout.canMove);
	SetCanAttack(startingLoadout.canAttack);
	SetCanBeDamaged(startingLoadout.canBeDamaged);
	SetHealthRegen(startingLoadout.healthRegen);
	team = startingLoadout.team;
	SetMovementModifier(startingLoadout.movementModifier);
	SetSpeed(startingLoadout.speed);
}

const TArray<AMech_RPGCharacter*>& AMech_RPGCharacter::GetCharacters() {
	return characters;
}

void AMech_RPGCharacter::AddWeapon(AWeapon* newWeapon) {
	if (newWeapon) {
		weapons.Add(newWeapon);
	}
}

TArray<UArmour*>& AMech_RPGCharacter::GetArmour() {
	return armour;
}

TEnumAsByte<GroupEnums::Role> AMech_RPGCharacter::GetRole()
{
	return startingRole;
}


bool AMech_RPGCharacter::GetInCombat()
{
	return inCombat;
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
	if (group != nullptr) {
		group->RemoveMember(this);

		if (!group->HasMemebers()) {
			group->ConditionalBeginDestroy();
		}
	}

	group = newVal;
	group->AddMemeber(this);
}

int32 AMech_RPGCharacter::GetID() {
	return id;
}

void AMech_RPGCharacter::SetID(int32 newVal) {
	id = newVal;
}

bool AMech_RPGCharacter::CompareGroup(UGroup* inGroup) {
	return GetGroup() != nullptr ? GetGroup()->Compare(inGroup) : true;
}

bool AMech_RPGCharacter::CompareGroup(AMech_RPGCharacter* inCharacter) {
	return inCharacter != nullptr && inCharacter->GetGroup() != nullptr ? CompareGroup(inCharacter->GetGroup()) : true;
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

float AMech_RPGCharacter::GetHealthChangeModifier() {
	return MAX(healthChangeModifier, 0.01F);
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

void AMech_RPGCharacter::SetHealthChangeModifier(float newVal) {
	healthChangeModifier = newVal;
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
	if (newAbility != nullptr) {
		abilities.Add(newAbility);
	}
}

UWidgetComponent * AMech_RPGCharacter::GetStats()
{
	return stats;
}