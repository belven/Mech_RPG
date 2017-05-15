#pragma once
#include "Mech_RPG.h"
#include "Abilities.h"
#include "Weapons.h"

#include "Items/ItemPickup.h"
#include "Items/Armour.h"
#include "BaseAIController.h"
#include "Blueprint/UserWidget.h"
#include "Mech_RPGPlayerController.h"
#include "UI/FloatingTextUI.h"
#include "UI/CharacterStats.h"
#include "Group.h"
#include "Abilities/Ability.h"
#include "Abilities/ChannelledAbility.h"
#include "Interactable.h"
#include "Quests/Quest.h"
#include "Quests/QuestManager.h"
#include "Math/UnrealMathUtility.h"
#include "Spawnpoints/Spawnpoint.h"
#include "Spawnpoints/PlayerSpawnpoint.h"
#include <limits>
#include "Delayed Events/EffectTimer.h"

TArray<AMech_RPGCharacter*> AMech_RPGCharacter::characters;
bool AMech_RPGCharacter::settingUpGroups = false;
const float AMech_RPGCharacter::lowHealth = 2000;
const float AMech_RPGCharacter::mediumHealth = AMech_RPGCharacter::lowHealth * 1.25;
const float AMech_RPGCharacter::highHealth = AMech_RPGCharacter::mediumHealth * 1.25;

AMech_RPGCharacter::AMech_RPGCharacter() :
	healthChangeModifier(1),
	inventory(NewObject<UInventory>(UInventory::StaticClass())),
	defenceModifier(0),
	speedModifier(1),
	startingRole(GroupEnums::DPS),
	health(2000),
	maxHealth(2000),
	channeling(false),
	team(TeamEnums::Paladins)
{
	static int32 ID = 0;
	SetID(ID++);
	SetActorTickEnabled(true);
	AIControllerClass = ABaseAIController::StaticClass();
	inventory = NewObject<UInventory>(UInventory::StaticClass());

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->AttachToComponent(RootComponent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	CameraBoom->bAbsoluteRotation = true; // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 1800.0f;
	CameraBoom->RelativeRotation = FRotator(-85.f, 0.f, 0.f);
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->AttachToComponent(CameraBoom, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false), USpringArmComponent::SocketName);
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
	GetCharacterMovement()->bCanWalkOffLedges = false;
	GetCharacterMovement()->SetWalkableFloorAngle(90);
	GetCharacterMovement()->SetAvoidanceGroup(0);
	GetCharacterMovement()->SetGroupsToAvoid(0);
	//GetCharacterMovement()->SetGroupsToIgnore(0);

	speed = GetCharacterMovement()->MaxWalkSpeed;

	static ConstructorHelpers::FClassFinder<UFloatingStats_BP> statsWidget(TEXT("/Game/TopDown/Blueprints/UI/CharacterUI/FloatingStats"));

	if (statsWidget.Succeeded()) {
		widgetClass = statsWidget.Class;
		stats = CreateDefaultSubobject<UWidgetComponent>(TEXT("Floating Stats Component"));
		stats->AttachToComponent(GetRootComponent(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	}

	static ConstructorHelpers::FClassFinder<UFloatingTextUI> floatingTextWidget(TEXT("/Game/TopDown/Blueprints/UI/CharacterUI/Floating_Text"));

	if (floatingTextWidget.Succeeded()) {
		floatingTextClass = floatingTextWidget.Class;
	}

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> newMesh(TEXT("/Game/TopDown/Meshes/Mech/Mech.Mech"));

	if (newMesh.Succeeded()) {
		GetMesh()->SetSkeletalMesh(newMesh.Object);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		GetMesh()->SetCollisionObjectType(mCharacterCollision);
	}

	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(60.0f, 140.0f);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GetCapsuleComponent()->SetCollisionObjectType(mCharacterCollision);
	GetCapsuleComponent()->SetCollisionResponseToChannel(mItemCollision, ECollisionResponse::ECR_Ignore);
	//GetCapsuleComponent()->SetCollisionResponseToChannel(mCharacterCollision, ECollisionResponse::ECR_Ignore);

	//radiusDection = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	//radiusDection->InitSphereRadius(1500.0f);
	//radiusDection->SetCollisionProfileName(TEXT("Pawn"));
	//radiusDection->AttachTo(this->RootComponent);
	//radiusDection->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	characters.Add(this);
}

AMech_RPGCharacter::~AMech_RPGCharacter() {
	characters.Remove(this);
	abilities.Empty();
	armour.Empty();

	if (GetCurrentWeapon() != nullptr) {
		GetCurrentWeapon()->Destroy();
		SetCurrentWeapon(nullptr);
	}
	
	if (GetGroup() != nullptr) {
		GetGroup()->RemoveMember(this);
	}

}

void AMech_RPGCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	if (!isDead) {
		GetCharacterMovement()->MaxWalkSpeed = speed * speedModifier;

		if (GetHealth() < GetMaxHealth()) {
			float regen = !inCombat ? GetMaxHealth() * 0.15 : healthRegen;
			health += regen * DeltaTime;

			GetFloatingStats()->UpdateHealthBar();

			if (GetCharacterStats() != nullptr) {
				GetCharacterStats()->UpdateHealthBar();
			}
		}

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

	CLAMP(health, GetMaxHealth(), 0);
}

void AMech_RPGCharacter::BeginPlay() {
	Super::BeginPlay();
	GetCharacterMovement()->SetAvoidanceEnabled(true);
	if (IsPendingKill()) {
		return;
	}

	if (!UseLoadout) {
		//CreatePresetRole(StartingRole());
	}
	else {
		SetupWithLoadout();
	}

	if (abilities.Num() > 0) {
		SetCurrentAbility(abilities[0]);
	}

	//SetUpGroup();
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
		trans.SetLocation(FVector(0.0, 8.0, 130.0));
		trans.SetScale3D(FVector(0.25, 0.75, 1));
		stats->SetRelativeTransform(trans);
		stats->AttachToComponent(GetRootComponent(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false));
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
	return *GetArmour().Find(pos);
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

bool AMech_RPGCharacter::IsAlly(AMech_RPGCharacter * other)
{
	return other->CompareGroup(GetGroup());
}

bool AMech_RPGCharacter::IsEnemy(AMech_RPGCharacter* other)
{
	return !other->CompareGroup(GetGroup());
}

float AMech_RPGCharacter::GetTotalResistance(EDamageType damageType) {
	float totalResistance = 0;
	for (ArmourMap armourFound : GetArmour()) {
		if (armourFound.Value != nullptr) totalResistance += armourFound.Value->GetResistance(damageType);
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

void AMech_RPGCharacter::RemoveQuest(UQuest* quest)
{
	quests.Remove(quest);
}

void AMech_RPGCharacter::ChangeHealth(FHealthChange healthChange) {
	if (GetGroup() != nullptr && !CompareGroup(healthChange.manipulator)) {
		GetGroup()->GroupMemberHit(healthChange.manipulator, this);
	}

	if (OnPreHealthChange.IsBound()) {
		OnPreHealthChange.Broadcast(healthChange);
	}

	if (healthChange.heals) {
		health += healthChange.changeAmount;
	}
	else if (canBeDamaged == 0) {
		float resistance = GetDefenceModifier();

		if (!healthChange.ignoresArmour) {
			resistance += (GetTotalResistance(healthChange.damageType) / 100);
		}

		resistance = MIN(0.99F, resistance);

		health -= (healthChange.changeAmount *= (1 - resistance));
	}

	PostHealthChange(healthChange);
}

void AMech_RPGCharacter::PostHealthChange(FHealthChange healthChange)
{
	CLAMP(health, GetMaxHealth(), 0);

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
		healthChange.manipulator->EnemyKilled(this);
		
		UQuestManager::EntityKilled(healthChange);
		SpawnItem(healthChange.manipulator);
	}

	UpdateStats();
}

void AMech_RPGCharacter::UpdateStats()
{
	if (GetFloatingStats() != nullptr) {
		GetFloatingStats()->UpdateHealthBar();
	}

	if (GetCharacterStats() != nullptr) {
		GetCharacterStats()->UpdateHealthBar();
	}
}

void AMech_RPGCharacter::SpawnItem(AMech_RPGCharacter* character)
{
	if (GetGroup()->GetPlayer() == nullptr) {
		ItemEnumns::ItemType type = UMiscLibrary::GetRandomEnum(ItemEnumns::Resource);
		AItem* newItem = CalucluateItemDrop(character->GetGroup(), type);
		AItemPickup::CreateItemPickup(newItem)->SetActorLocation(GetActorLocation());
	}
}

AItem* AMech_RPGCharacter::CalucluateItemDrop(UGroup* inGroup, ItemEnumns::ItemType type) {
	TMultiMap<int32, int32> gradeMap;

	int32 outputGrade = 0;
	int32 outputQuality = 0;

	bool upgradeGrade = UMiscLibrary::IsSuccess(70);// upgradeGradeChance;
	bool upgradeQuality = UMiscLibrary::IsSuccess(70); //upgradeQualityChance;

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

	for (TPair<int32, int32>& map : gradeMap) {
		// Found a higher quantity
		if (map.Value > heighestValue.Value) {
			heighestValue = map;
		}
		// Found the same quantity, only set if the grade is higher
		else if (map.Value == heighestValue.Value && map.Key > heighestValue.Key) {
			heighestValue = map;
		}
	}

	if (upgradeGrade)
		meanGrade++;

	if (upgradeQuality)
		meanQuality++;

	outputGrade = FMath::RoundHalfToEven(MAX(meanGrade, heighestValue.Key));
	outputQuality = FMath::RoundHalfToEven(meanQuality);

	AItem* newItem = AItem::CreateItemByType(type, GetWorld(), outputGrade, outputQuality);

	if (newItem != nullptr) {
		newItem->SetItemOwner(this);
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

void AMech_RPGCharacter::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	if (isPlayer) {
		//UQuestManager::GetQuests().Empty();
	}
}

void AMech_RPGCharacter::SetInCombat(AMech_RPGCharacter* attacker, AMech_RPGCharacter* damagedMember)
{
	if (!inCombat) {
		inCombat = true;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_OutOfCombat, this, &AMech_RPGCharacter::OutOfCombat, 1.0F, true);
		stats->SetVisibility(true, true);
	}
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
	else {
		inventory = NewObject<UInventory>(UInventory::StaticClass());
	}

	MaximiseHealth();
	
	channeling = false;
	inCombat = false;

	canAttack = 0;
	canMove = 0;
	canBeDamaged = 0;
}

void AMech_RPGCharacter::MaximiseHealth()
{
	SetHealth(GetMaxHealth());
}

void AMech_RPGCharacter::ResetInvunrelbility()
{
	ApplyCrowdControl(EffectEnums::Damage, true);
}

AItem* AMech_RPGCharacter::AddItem(AItem* itemToAdd)
{
	AItem* item = GetInventory()->AddItem(itemToAdd);
	itemToAdd->SetItemOwner(this);

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
	for (AMech_RPGCharacter* character : UMiscLibrary::GetCharactersInRange(3000, this->GetActorLocation())) {
		if (!character->IsDead() && character->IsEnemy(this) && character->inCombat) {
			return;
		}
	}

	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_OutOfCombat);

	inCombat = false;
	OnStopFiring.Broadcast();

	if (OnOutOfCombat.IsBound()) OnOutOfCombat.Broadcast();

	stats->SetVisibility(false, true);

	if (IsDead() && GetGroup()->GetPlayer() != nullptr) {
		Resurrect();
	}
	else if (IsDead()) {
		Reset();
		SetInvunrebleTimer();

	}
}

void AMech_RPGCharacter::SetInvunrebleTimer()
{
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_Invunrelbility, this, &AMech_RPGCharacter::RemoveFromPlay, 6.0F);
}

void AMech_RPGCharacter::Resurrect()
{
	SetDead(false);
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);

	FHealthChange change;
	change.heals = true;
	change.changeAmount = GetMaxHealth();
	change.manipulator = this;
	change.target = this;
	ChangeHealth(change);

	TArray<TEnumAsByte<EffectEnums::CrowdControl>> effects;
	effects.Add(EffectEnums::Damage);

	UEffectTimer::CreateEffectTimer(this, 3.0F, effects);
	ApplyCrowdControl(EffectEnums::Damage, false);
	//FindSpawnpoint();

	GetWorld()->GetTimerManager().SetTimer(TimerHandle_Invunrelbility, this, &AMech_RPGCharacter::ResetInvunrelbility, 3.0F);
}

void AMech_RPGCharacter::FindSpawnpoint()
{
	float distanceTo = std::numeric_limits<float>::max();
	bool locationFound = false;

	APlayerSpawnpoint* spawn = nullptr;
	TArray<ASpawnpoint*>* spawnpoints = ASpawnpoint::GetSpawnpoints(APlayerSpawnpoint::StaticClass());

	if (spawnpoints != nullptr) {
		for (ASpawnpoint* spawnpoint : *spawnpoints) {
			if (spawnpoint != nullptr) {
				APlayerSpawnpoint* spawnFound = Cast<APlayerSpawnpoint>(spawnpoint);

				if (spawnFound != nullptr
					&& (spawn == nullptr || GetDistanceTo(spawnFound) < distanceTo)) {
					distanceTo = GetDistanceTo(spawnFound);
					spawn = spawnFound;
				}
			}
		}

		if (spawn != nullptr) {
			SetActorLocation(UMiscLibrary::FindNavLocation(this, ASpawnpoint::defaultSpawnRadius));
			ASpawnpoint::AdjustCharacterLocationByCapsule(this);
		}
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

	Reset();

	StartingRole(inRole);

	isPlayer = mIsChildOf(GetController(), AMech_RPGPlayerController::StaticClass());

	switch (inRole) {
	case GroupEnums::DPS:
		SetCurrentWeapon(mCreatePresetWeapon(WeaponEnums::SMG, grade, quaility));
		AddAbility(UAbility::CreateChannelledPresetAbility(this, AbilityEnums::Grenade, 1.75F, true, true));
		AddAbility(mCreatePresetAbility(AbilityEnums::CritBoost));
		blastResistance = mGetDefaultArmourValue(ArmourGrades::Light);
		phsyicalResistance = mGetDefaultArmourValue(ArmourGrades::Light);
		energyResistance = mGetDefaultArmourValue(ArmourGrades::Light);
		SetMaxHealth(lowHealth);
		break;

	case GroupEnums::Healer:
		SetCurrentWeapon(mCreatePresetWeapon(WeaponEnums::Bio_Repair, grade, quaility));
		//AddAbility(mCreatePresetAbility(AbilityEnums::Heal));
		AddAbility(mCreatePresetAbility(AbilityEnums::AoEHeal));
		AddAbility(UTimedHealthChange::CreateTimedHealthChange(this, 10.0F, 1.0F, 2.0F, 16.0F, true));
		blastResistance = mGetDefaultArmourValue(ArmourGrades::Light);
		phsyicalResistance = mGetDefaultArmourValue(ArmourGrades::Light);
		energyResistance = mGetDefaultArmourValue(ArmourGrades::Light);
		SetMaxHealth(lowHealth);
		break;

	case GroupEnums::Tank:
		SetCurrentWeapon(mCreatePresetWeapon(WeaponEnums::Sword, grade, quaility));
		AddAbility(mCreatePresetAbility(AbilityEnums::Taunt));
		AddAbility(mCreatePresetAbility(AbilityEnums::Shield));
		blastResistance = mGetDefaultArmourValue(ArmourGrades::Light);
		phsyicalResistance = mGetDefaultArmourValue(ArmourGrades::Light);
		energyResistance = mGetDefaultArmourValue(ArmourGrades::Light);
		SetMaxHealth(lowHealth);
		break;

	case GroupEnums::Sniper:
		SetCurrentWeapon(ALaserSniper::CreateLaserSniper(GetWorld(), this));
		AddAbility(UAbility::CreateChannelledPresetAbility(this, AbilityEnums::Snipe, 2.5F, false, true));
		AddAbility(mCreatePresetAbility(AbilityEnums::CritBoost));
		blastResistance = mGetDefaultArmourValue(ArmourGrades::Light);
		phsyicalResistance = mGetDefaultArmourValue(ArmourGrades::Light);
		energyResistance = mGetDefaultArmourValue(ArmourGrades::Light);
		SetMaxHealth(lowHealth);
		break;

	case GroupEnums::Support:
		SetCurrentWeapon(mCreatePresetWeapon(WeaponEnums::Shotgun, grade, quaility));
		AddAbility(USummonDamageDrone::CreateAbility(20, this));
		AddAbility(mCreatePresetAbility(AbilityEnums::Shield));
		AddAbility(mCreatePresetAbility(AbilityEnums::Disable));
		blastResistance = mGetDefaultArmourValue(ArmourGrades::Light);
		phsyicalResistance = mGetDefaultArmourValue(ArmourGrades::Light);
		energyResistance = mGetDefaultArmourValue(ArmourGrades::Light);
		SetMaxHealth(lowHealth);
		break;

	default:
		CreatePresetRole(GroupEnums::DPS);
		break;
	}

	MaximiseHealth();

	CreateArmour(phsyicalResistance, blastResistance, energyResistance, grade, quaility);
}

void AMech_RPGCharacter::CreateArmour(float phsyicalResistance, float blastResistance, float energyResistance, int32 grade, int32 quaility)
{
	for (int i = 0; i < ArmourEnums::End; i++) {
		ArmourEnums::ArmourPosition pos = (ArmourEnums::ArmourPosition)i;
		AArmour* newArmour = AArmour::CreateArmour(GetWorld(), "Test", phsyicalResistance, blastResistance, energyResistance, pos, this, grade, quaility);
		GetArmour().Add(pos, newArmour);
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
	MaximiseHealth();
	SetDefenceModifier(startingLoadout.defenceModifier);
	SetHealthChangeModifier(startingLoadout.damageModifier);
	SetCanMove(startingLoadout.canMove);
	SetCanAttack(startingLoadout.canAttack);
	SetCanBeDamaged(startingLoadout.canBeDamaged);
	SetHealthRegen(startingLoadout.healthRegen);
	SetTeam(startingLoadout.team);
	SetSpeedModifier(startingLoadout.movementModifier);
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
		ABaseAIController* con = Cast<ABaseAIController>(Controller);
		if (group != nullptr) {
			group->RemoveMember(this);
			group->OnMemberDamageEvent.RemoveAll(this);

			if (con != nullptr) {
				group->OnMemberDamageEvent.RemoveAll(con);
			}

			if (!group->HasMemebers()) {
				group->ConditionalBeginDestroy();
			}
		}

		group = newVal;
		group->AddMemeber(this);

		if (group != nullptr) {
			group->OnMemberDamageEvent.AddUniqueDynamic(this, &AMech_RPGCharacter::SetInCombat);

			if (con != nullptr) {
				group->OnMemberDamageEvent.AddUniqueDynamic(con, &ABaseAIController::GroupMemberDamaged);
			}
		}
	}
}

bool AMech_RPGCharacter::CompareGroup(UGroup* inGroup) {
	return GetGroup() != nullptr ? GetGroup()->Compare(inGroup) : true;
}

bool AMech_RPGCharacter::CompareGroup(AMech_RPGCharacter* inCharacter) {
	if (UMiscLibrary::IsCharacterAlive(inCharacter)
		&& inCharacter->GetGroup() != nullptr) {
		return CompareGroup(inCharacter->GetGroup());
	}
	return true;
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
