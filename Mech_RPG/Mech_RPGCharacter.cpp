#pragma once
#include "Mech_RPG.h"
#include "Abilities.h"
#include "Weapons.h"

#include "ItemPickup.h"
#include "Engine.h"
#include "Armour.h"
#include "BaseAIController.h"
#include "AllyAIController.h"
#include "Blueprint/UserWidget.h"
#include "Mech_RPGPlayerController.h"
#include "FloatingTextUI.h"
#include "CharacterStats.h"
#include "Group.h"
#include "Abilities/Ability.h"
#include "Abilities/ChannelledAbility.h"
#include "Interactable.h"
#include "Quests/Quest.h"
#include "Mech_RPGPlayerController.h"
#include "Math/UnrealMathUtility.h"
#include "Map.h"

#define mCreatePresetWeapon(type, grade, quailty) AWeapon::CreatePresetWeapon(GetWorld(), this, type, grade, quailty)
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

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	StartingRole(GroupEnums::DPS);
	healthChangeModifier = 1;
	defenceModifier = 0;
	movementModifier = 1.0;

	SetMaxHealth(2000);
	SetHealth(GetMaxHealth());

	speed = GetCharacterMovement()->MaxWalkSpeed;
	GetCharacterMovement()->bCanWalkOffLedges = false;
	channeling = false;
	SetTeam(TeamEnums::Paladins);

	static ConstructorHelpers::FClassFinder<UFloatingStats_BP> statsWidget(TEXT("/Game/TopDown/Blueprints/UI/CharacterUI/FloatingStats"));

	if (statsWidget.Succeeded()) {
		widgetClass = statsWidget.Class;
		stats = CreateDefaultSubobject<UWidgetComponent>(TEXT("Floating Stats Component"));
		stats->AttachTo(GetRootComponent());
	}

	static ConstructorHelpers::FClassFinder<UFloatingTextUI> floatingTextWidget(TEXT("/Game/TopDown/Blueprints/UI/CharacterUI/Floating_Text"));

	if (floatingTextWidget.Succeeded()) {
		floatingTextClass = floatingTextWidget.Class;
	}

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> newMesh(TEXT("/Game/TopDown/Meshes/Mecha_2.Mecha_2"));

	if (newMesh.Succeeded()) {
		GetMesh()->SetSkeletalMesh(newMesh.Object);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}

	AIControllerClass = ABaseAIController::StaticClass();
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	characters.Add(this);
}

AMech_RPGCharacter::~AMech_RPGCharacter() {
	characters.Remove(this);
	abilities.Empty();
	//weapons.Empty();
	armour.Empty();

	if (GetCurrentWeapon() != nullptr) {
		GetCurrentWeapon()->Destroy();
		SetCurrentWeapon(nullptr);
	}

	if (mIsChildOf(GetController(), AMech_RPGPlayerController::StaticClass())) {
		UMiscLibrary::SetPlayerController(nullptr);
	}
}

void AMech_RPGCharacter::PossessedBy(AController* NewController) {
	Super::PossessedBy(NewController);
	AMech_RPGPlayerController* con = Cast<AMech_RPGPlayerController>(NewController);

	if (con) {
		con->SetOwner(this);
		UMiscLibrary::SetPlayerController(con);
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

		if (GetHealth() < GetMaxHealth()) {
			float regen = !inCombat ? GetMaxHealth() * 0.15 : healthRegen;
			health += regen * DeltaTime;

			GetFloatingStats()->UpdateHealthBar();

			if (GetCharacterStats() != nullptr) {
				GetCharacterStats()->UpdateHealthBar();
			}
		}

		CLAMP(health, GetMaxHealth(), 0);

		if (stats->GetUserWidgetObject() != nullptr && stats->GetUserWidgetObject() != nullptr) {
			if (mIsChildOf(GetController(), AMech_RPGPlayerController::StaticClass()) && GetTopDownCamera() != nullptr) {
				UMiscLibrary::SetCameraRot(FRotator(-GetTopDownCamera()->GetComponentRotation().Pitch, UMiscLibrary::GetWidgetYaw(GetTopDownCamera()) + 90, 0));
				GetStats()->SetWorldRotation(UMiscLibrary::GetCameraRot());
			}
			else if (GetStats() != nullptr) {
				GetStats()->SetWorldRotation(UMiscLibrary::GetCameraRot());
			}
		}
	}
}

void AMech_RPGCharacter::BeginPlay() {
	Super::BeginPlay();
	if (IsPendingKill()) {
		return;
	}

	inventory = NewObject<UInventory>(UInventory::StaticClass());

	if (!UseLoadout) {
		CreatePresetRole(StartingRole());
	}
	else {
		SetupWithLoadout();
	}

	if (abilities.Num() > 0) {
		SetCurrentAbility(abilities[0]);
	}

	SetUpGroup();

	SetUpWidgets();

	if (OnPostBeginPlay.IsBound()) {
		OnPostBeginPlay.Broadcast(this);
	}
}

void AMech_RPGCharacter::SetUpWidgets() {
	if (widgetClass != nullptr) {
		floatingStats = CreateWidget<UFloatingStats_BP>(GetWorld(), widgetClass);
		floatingStats->SetOwner(this);
		stats->SetWidget(floatingStats);

		FTransform trans;
		trans.SetLocation(FVector(-0.000035, 7.999790, 111.999756));
		trans.SetScale3D(FVector(0.25, 0.75, 1));
		stats->SetRelativeTransform(trans);
		stats->AttachTo(GetRootComponent());
		stats->SetDrawSize(FVector2D(100, 50));

		//if (!isPlayer && !isAlly) {
		stats->SetVisibility(false, true);
		//}
	}

	if (inventory != nullptr) {
		int invSize = 20;
		inventory->SetMaxItemCount(invSize);
		/*inventory->AddItem(AItem::CreateItem(GetWorld(), this, "Item 1", 3, 0, 0, 5));
		inventory->AddItem(AItem::CreateItem(GetWorld(), this, "Item 1", 3, 0, 0, 5));
		inventory->AddItem(AItem::CreateItem(GetWorld(), this, "Item 2", 4, 0, 0, 2));
		inventory->AddItem(AItem::CreateItem(GetWorld(), this, "Item 3", 0, 0, 0, 2));
		inventory->AddItem(AItem::CreateItem(GetWorld(), this, "Item 4", 3, 0, 0, 1));*/
	}

	GetFloatingStats()->UpdateHealthBar();

	if (GetCharacterStats() != nullptr) {
		GetCharacterStats()->UpdateHealthBar();
	}
}

void AMech_RPGCharacter::SetCharacterStats(class UCharacterStats* val) {
	characterStats = val;

	if (GetCharacterStats() != nullptr) {
		GetCharacterStats()->UpdateHealthBar();
	}
}

AArmour* AMech_RPGCharacter::GetArmourByPosition(TEnumAsByte<ArmourEnums::ArmourPosition> pos) {
	for (AArmour* armour : GetArmour()) {
		if (armour != nullptr && armour->GetArmourPosition() == pos) return armour;
	}
	return nullptr;
}

void AMech_RPGCharacter::SetUpGroup() {
	if (group == nullptr) {
		TArray<AMech_RPGCharacter*> charactersFound = UMiscLibrary::GetCharactersInRange(700, this);

		for (AMech_RPGCharacter* character : charactersFound) {
			if (character != this && character->GetTeam() == GetTeam() && character->GetGroup() != nullptr) {
				SetGroup(character->GetGroup());
				break;
			}
		}
	}

	if (group == nullptr) {
		SetGroup(UGroup::CreateGroup(GetTeam()));
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

float AMech_RPGCharacter::GetTotalResistance(DamageEnums::DamageType damageType) {
	float totalResistance = 0;
	for (AArmour* armour : armour) {
		if (armour != nullptr) totalResistance += armour->GetResistance(damageType);
	}

	return MAX(totalResistance, 1);
}

void AMech_RPGCharacter::AddQuest(UQuest * newQuest)
{
	for (UQuest* quest : GetQuests())
	{
		if (quest->GetQuestName().Equals(newQuest->GetQuestName())) {
			return;
		}
	}

	quests.Add(newQuest);
	newQuest->SetOwner(this);
	OnQuestAdded.Broadcast(newQuest);
}

void AMech_RPGCharacter::AbandonQuest(UQuest* quest)
{
	quests.Remove(quest);
}

void AMech_RPGCharacter::ChangeHealth(FHealthChange healthChange) {
	if (GetGroup() != nullptr && !CompareGroup(healthChange.damager)) {
		GetGroup()->GroupMemberHit(healthChange.damager, this);
	}

	if (OnPreHealthChange.IsBound()) {
		OnPreHealthChange.Broadcast(healthChange);
	}

	if (healthChange.heals) {
		health += healthChange.healthChange;
	}
	else if (canBeDamaged == 0) {
		float resistance = (GetTotalResistance(healthChange.damageType) / 100);

		resistance *= (1 + GetDefenceModifier());
		resistance = MIN(0.99F, resistance);
		health -= (healthChange.healthChange *= (1 - resistance));
	}

	PostHealthChange(healthChange);
}

void AMech_RPGCharacter::PostHealthChange(FHealthChange healthChange)
{
	if (OnPostHealthChange.IsBound()) {
		OnPostHealthChange.Broadcast(healthChange);
	}

	UFloatingTextUI::CreateFloatingText(floatingTextClass, healthChange);

	if (health <= 0) {
		health = 0;
		SetDead(true);
		SetActorHiddenInGame(true);
		if (GetCurrentWeapon() != nullptr) GetCurrentWeapon()->SetActorHiddenInGame(true);
		OnStopFiring.Broadcast();
		healthChange.damager->EnemyKilled(this);

		if (GetGroup()->GetPlayer() == nullptr) {
			AItemPickup::CreateItemPickup(CalucluateItemDrop(healthChange.damager->GetGroup(), GetCurrentWeapon()->GetType()))->SetActorLocation(GetActorLocation());
		}
	}

	if (GetFloatingStats() != nullptr) {
		GetFloatingStats()->UpdateHealthBar();
	}

	if (GetCharacterStats() != nullptr) {
		GetCharacterStats()->UpdateHealthBar();
	}
}

AItem* AMech_RPGCharacter::CalucluateItemDrop(UGroup* inGroup, ItemEnumns::ItemType type) {
	TMultiMap<int32, int32> gradeMap;
	int32 outputGrade;
	int32 outputQuality;
	bool upgradeGrade = FMath::RandHelper(100) <= 70;// upgradeGradeChance;
	bool upgradeQuality = FMath::RandHelper(100) <= 70; //upgradeQualityChance;

	float totalItems = 0;
	float lowestGrade = AItem::HighestItemLevel;
	float totalGrade = 0;
	float meanGrade = 0;
	int32 modeGrade = 0;

	float lowestQuality = 20;
	float totalQuality = 0;
	float meanQuality = 0;
	int32 modeQuality = 0;

	for (AMech_RPGCharacter* member : inGroup->GetMembers()) {
		//for (AItem* item : member->GetInventory()->GetItems()) {
		AItem* item = member->GetCurrentWeapon();
		if (item != nullptr && item->GetType() == type) {
			totalItems++;
			totalGrade += item->GetGrade();
			totalQuality += item->GetQuality();

			if (!gradeMap.Contains(item->GetGrade())) {
				gradeMap.Add(item->GetGrade(), 1);
			}
			else {
				gradeMap.Add(item->GetGrade(), (int32)(*gradeMap.Find(item->GetGrade()) + 1));
			}

			if (item->GetQuality() < lowestQuality) {
				lowestQuality = item->GetQuality();
			}

			if (item->GetGrade() < lowestGrade) {
				lowestGrade = item->GetGrade();
			}
		}
		//}
	}

	meanGrade = totalGrade / totalItems;
	meanQuality = totalQuality / totalItems;

	TPair<int32, int32> heighestValue;
	heighestValue.Key = 1;
	heighestValue.Value = 0;

	for (auto& map : gradeMap) {
		// Found a higher quantity
		if (map.Value > heighestValue.Value) {
			heighestValue = map;
		}
		// Found the same quantity, only set if the grade is higher
		else if (map.Value == heighestValue.Value && map.Key > heighestValue.Key) {
			heighestValue = map;
		}
	}

	//if (upgradeGrade) 
	meanGrade++;
	//if (upgradeQuality)
	meanQuality++;

	outputGrade = FMath::RoundHalfToEven(MAX(meanGrade, heighestValue.Key));
	outputQuality = FMath::RoundHalfToEven(meanQuality);

	AItem* newItem = AItem::CreateItemByType(type, GetWorld(), outputGrade, outputQuality);

	if (newItem != nullptr) {
		newItem->SetOwner(this);
		return newItem;
	}

	return nullptr;
}

void AMech_RPGCharacter::EnemyKilled(AMech_RPGCharacter* character)
{
	if (OnEnemyKilled.IsBound()) OnEnemyKilled.Broadcast(character);
	GetGroup()->GroupEnemyKilled(character);
}

void AMech_RPGCharacter::SetActorHiddenInGame(bool bNewHidden)
{
	Super::SetActorHiddenInGame(bNewHidden);
	SetActorEnableCollision(!bNewHidden);

	if (GetCurrentWeapon() != nullptr) {
		GetCurrentWeapon()->SetActorHiddenInGame(bNewHidden);
	}
}

void AMech_RPGCharacter::SetInCombat(AMech_RPGCharacter* attacker, AMech_RPGCharacter* damagedMember) {
	inCombat = true;
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_OutOfCombat);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_OutOfCombat, this, &AMech_RPGCharacter::OutOfCombat, 3.0F);

	//if (!isPlayer && !isAlly) {
	stats->SetVisibility(true, true);
	//}
}

void AMech_RPGCharacter::Reset()
{
	if (GetCurrentWeapon() != nullptr) {
		GetCurrentWeapon()->Destroy();
		SetCurrentWeapon(nullptr);
	}

	abilities.Empty();
	armour.Empty();

	if (GetInventory() != nullptr) {
		GetInventory()->GetItems().Empty();
	}

	inventory = NewObject<UInventory>(UInventory::StaticClass());

	SetHealth(GetMaxHealth());

	channeling = false;
	inCombat = false;

	canAttack = 0;
	canMove = 0;
	canBeDamaged = 0;
}

void AMech_RPGCharacter::ResetInvunrelbility()
{
	ApplyCrowdControl(EffectEnums::Damage, true);
}

AItem* AMech_RPGCharacter::AddItem(AItem* itemToAdd)
{
	AItem* item = GetInventory()->AddItem(itemToAdd);
	itemToAdd->SetOwner(this);

	if (GetGroup() != nullptr) {
		GetGroup()->ItemPickup(itemToAdd);
	}

	if (OnItemPickUpEvent.IsBound()) {
		OnItemPickUpEvent.Broadcast(itemToAdd);
	}
	return item;
}

void AMech_RPGCharacter::NPCInteract(AMech_RPGCharacter* character)
{
	if (character->GetQuests().Num() > 0) {
		for (UQuest* quest : character->GetQuests()) {
			if (!GetQuests().Contains(quest)) {
				AddQuest(quest);
			}
		}
	}

	if (OnNPCInteractEvent.IsBound()) {
		OnNPCInteractEvent.Broadcast(character);
	}

	GetGroup()->NPCInteract(character);
}

void AMech_RPGCharacter::Interact(AInteractable * interactable)
{
	interactable->Interact(this);

	if (OnInteractEvent.IsBound()) {
		OnInteractEvent.Broadcast(interactable);
	}

	GetGroup()->Interact(interactable);
}

void AMech_RPGCharacter::NotifyActorBeginCursorOver()
{
	if (UMiscLibrary::GetPlayerController() != nullptr) {
		UMiscLibrary::GetPlayerController()->MouseOverActor(this, TargetEnums::Character);
	}
}

void AMech_RPGCharacter::OutOfCombat() {
	inCombat = false;
	OnStopFiring.Broadcast();

	if (OnOutOfCombat.IsBound()) OnOutOfCombat.Broadcast();

	//if (!isPlayer && !isAlly) {
	stats->SetVisibility(false, true);
	//}

	if (IsDead() && GetGroup()->GetPlayer() != nullptr) {
		SetDead(false);
		SetActorHiddenInGame(false);
		SetActorEnableCollision(true);

		SetHealth(GetMaxHealth() * 0.2);
		ApplyCrowdControl(EffectEnums::Damage, false);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_Invunrelbility, this, &AMech_RPGCharacter::ResetInvunrelbility, 3.0F);
	}
	else if (IsDead()) {
		Reset();
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_Invunrelbility, this, &AMech_RPGCharacter::RemoveFromPlay, 6.0F);
	}
}

void AMech_RPGCharacter::RemoveFromPlay()
{
	stats->DestroyComponent();
	group->RemoveMember(this);
	//inventory->BeginDestroy();

	if (GetCurrentWeapon() != nullptr) {
		GetCurrentWeapon()->Destroy();
	}
	Destroy();
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
	float blastResistance = 5;
	float phsyicalResistance = 5;
	float energyResistance = 5;
	float statModifier = 0;
	static float lowHealth = 2000;
	static float mediumHealth = lowHealth * 1.25;
	static float highHealth = mediumHealth * 1.25;

	Reset();

	StartingRole(inRole);

	isPlayer = mIsChildOf(GetController(), AMech_RPGPlayerController::StaticClass());
	isAlly = mIsChildOf(GetController(), AAllyAIController::StaticClass());

	if (isAlly || isPlayer) {
		statModifier = GetModifierForDifficulty(UMiscLibrary::GetDifficulty());
	}

	switch (inRole) {
	case GroupEnums::DPS:
		SetCurrentWeapon(mCreatePresetWeapon(WeaponEnums::SMG, grade, quaility));
		AddAbility(UAbility::CreateChannelledPresetAbility(this, AbilityEnums::Grenade, 1.75F, true, false));
		AddAbility(UTimedHealthChange::CreateTimedHealthChange(this, 10.0F, 200.0F));
		SetDefenceModifier(0.0F + statModifier);
		SetHealthChangeModifier(1.0F + statModifier);
		blastResistance = AArmour::GetDeafultValue(ArmourGrades::Light);
		phsyicalResistance = AArmour::GetDeafultValue(ArmourGrades::MediumLight);
		energyResistance = AArmour::GetDeafultValue(ArmourGrades::Light);
		SetMaxHealth(lowHealth * (1 + statModifier));
		break;

	case GroupEnums::Healer:
		SetCurrentWeapon(mCreatePresetWeapon(WeaponEnums::Bio_Repair, grade, quaility));
		AddAbility(mCreatePresetAbility(AbilityEnums::Heal));
		AddAbility(mCreatePresetAbility(AbilityEnums::AoEHeal));
		AddAbility(UTimedHealthChange::CreateTimedHealthChange(this, 10.0F, 200.0F));
		SetDefenceModifier(0.0F + statModifier);
		SetHealthChangeModifier(1.0F + statModifier);
		SetMovementModifier(1.0F + statModifier);
		blastResistance = AArmour::GetDeafultValue(ArmourGrades::Light);
		phsyicalResistance = AArmour::GetDeafultValue(ArmourGrades::MediumLight);
		energyResistance = AArmour::GetDeafultValue(ArmourGrades::Light);
		SetMaxHealth(lowHealth * (1 + statModifier));
		break;

	case GroupEnums::Tank:
		SetCurrentWeapon(mCreatePresetWeapon(WeaponEnums::Shotgun, grade, quaility));
		AddAbility(mCreatePresetAbility(AbilityEnums::Taunt));
		AddAbility(mCreatePresetAbility(AbilityEnums::Stun));
		SetDefenceModifier(0.0F + statModifier);
		SetHealthChangeModifier(1.0F + statModifier);
		SetMovementModifier(1.0F + statModifier);
		blastResistance = AArmour::GetDeafultValue(ArmourGrades::MediumHeavy);
		phsyicalResistance = AArmour::GetDeafultValue(ArmourGrades::Heavy);
		energyResistance = AArmour::GetDeafultValue(ArmourGrades::Medium);
		SetMaxHealth(highHealth * (1 + statModifier));
		break;

	case GroupEnums::Sniper:
		SetCurrentWeapon(ALaserSniper::CreateLaserSniper(GetWorld(), this));
		AddAbility(UAbility::CreateChannelledPresetAbility(this, AbilityEnums::Snipe, 2.5F, false, false));
		AddAbility(mCreatePresetAbility(AbilityEnums::CritBoost));
		SetDefenceModifier(0.0F + statModifier);
		SetHealthChangeModifier(1.0F + statModifier);
		blastResistance = AArmour::GetDeafultValue(ArmourGrades::Light);
		phsyicalResistance = AArmour::GetDeafultValue(ArmourGrades::MediumLight);
		energyResistance = AArmour::GetDeafultValue(ArmourGrades::Light);
		SetMaxHealth(lowHealth * (1 + statModifier));
		break;

	case GroupEnums::Support:
		SetCurrentWeapon(mCreatePresetWeapon(WeaponEnums::Shotgun, grade, quaility));
		AddAbility(USummonDamageDrone::CreateAbility(20, this));
		AddAbility(mCreatePresetAbility(AbilityEnums::Shield));
		AddAbility(mCreatePresetAbility(AbilityEnums::Disable));
		SetDefenceModifier(0.0F + statModifier);
		SetHealthChangeModifier(1.0F + statModifier);
		blastResistance = AArmour::GetDeafultValue(ArmourGrades::MediumLight);
		phsyicalResistance = AArmour::GetDeafultValue(ArmourGrades::Medium);
		energyResistance = AArmour::GetDeafultValue(ArmourGrades::MediumLight);
		SetMaxHealth(mediumHealth * (1 + statModifier));
		break;

	default:
		CreatePresetRole(GroupEnums::DPS);
		break;
	}

	SetHealth(GetMaxHealth());

	for (int i = 0; i < ArmourEnums::End; i++) {
		AArmour* newArmour = AArmour::CreateArmour(GetWorld(), "Test", phsyicalResistance, blastResistance, energyResistance, (ArmourEnums::ArmourPosition)i,  this, grade, quaility);
		GetArmour().Add(newArmour);
		AddItem(newArmour);
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
	SetTeam(startingLoadout.team);
	SetMovementModifier(startingLoadout.movementModifier);
	SetSpeed(startingLoadout.speed);
}

void AMech_RPGCharacter::SetDead(bool newVal) {
	isDead = newVal;
}

void AMech_RPGCharacter::SetCurrentWeapon(AWeapon* newVal) {
	if (newVal != nullptr && currentWeapon != newVal) {
		if (!GetInventory()->GetItems().Contains(newVal)) {
			AddItem(newVal);
		}

		if (GetCurrentWeapon() != nullptr) {
			GetCurrentWeapon()->Unequip();
		}

		if (OnSwappedWeapons.IsBound()) {
			OnSwappedWeapons.Broadcast(currentWeapon, newVal);
		}

		newVal->Equip();
	}

	currentWeapon = newVal;
}

void AMech_RPGCharacter::SetGroup(UGroup* newVal) {
	if (newVal != nullptr) {
		if (group != nullptr) {
			group->RemoveMember(this);

			if (!group->HasMemebers()) {
				group->ConditionalBeginDestroy();
			}
		}

		group = newVal;
		group->AddMemeber(this);
	}
}

bool AMech_RPGCharacter::CompareGroup(UGroup* inGroup) {
	return GetGroup() != nullptr ? GetGroup()->Compare(inGroup) : true;
}

bool AMech_RPGCharacter::CompareGroup(AMech_RPGCharacter* inCharacter) {
	return this != nullptr && inCharacter != nullptr && inCharacter->GetGroup() != nullptr ? CompareGroup(inCharacter->GetGroup()) : true;
}

void AMech_RPGCharacter::AddAbility(UAbility* newAbility) {
	if (newAbility != nullptr) {
		abilities.Add(newAbility);

		if (GetCharacterStats() != nullptr) {
			GetCharacterStats()->UpdateAbilityBar();
		}
	}
}

void AMech_RPGCharacter::RemoveAbility(UAbility* newAbility)
{
	if (newAbility != nullptr && abilities.Contains(newAbility)) {
		abilities.Remove(newAbility);

		if (GetCharacterStats() != nullptr) {
			GetCharacterStats()->UpdateAbilityBar();
		}
	}
}