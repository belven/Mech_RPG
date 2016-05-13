UENUM(BlueprintType)
namespace TeamEnums {
	enum Team {
		Paladins,
		Mercenaries
	};
}

UENUM(BlueprintType)
namespace EffectEnums {
	enum CrowdControl {
		Attack,
		Move,
		Damage,
		Cast
	};
}

// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "Tag.h"
#include "Weapon.h"
#include "Group.h"
#include "Array.h"
#include "Abilities/Ability.h"
#include "Inventory.h"
#include "FloatingStats_BP.h"
#include "Mech_RPGCharacter.generated.h"

USTRUCT(BlueprintType)
struct FHealthChange {
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		AMech_RPGCharacter* target = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		AMech_RPGCharacter* damager = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		AWeapon* weaponUsed = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		float healthChange = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		TEnumAsByte<DamageEnums::DamageType> damageType = DamageEnums::Physical;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		bool heals = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		bool crit = false;
};

USTRUCT(BlueprintType)
struct FLoadout {
	GENERATED_USTRUCT_BODY()
public:
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loadout")
	//TArray<AWeapon*> weapons;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loadout")
	//TArray<UAbility*> abilities;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loadout")
		float damageModifier = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loadout")
		float defenceModifier = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loadout")
		float maxHealth = 2000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loadout")
		float healthRegen = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loadout")
		TEnumAsByte<TeamEnums::Team> team = TeamEnums::Paladins;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loadout")
		int32 canAttack = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loadout")
		int32 canMove = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loadout")
		int32 canBeDamaged = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loadout")
		float speed = 600;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loadout")
		float movementModifier = 1;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPreHealthChangeEvent, FHealthChange, healthChange);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPostHealthChangeEvent, FHealthChange, healthChange);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPostBeginPlay, AMech_RPGCharacter*, character);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEnemyKilled, AMech_RPGCharacter*, character);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNPCInteractEvent, AMech_RPGCharacter*, character);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FItemPickUpEvent, AItem*, item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInteractEvent, AInteractable*, interactable);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FQuestAdded, UQuest*, quest);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStopFiring);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOutOfCombat);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSwappedWeapons, AWeapon*, oldWeapon, AWeapon*, newWeapon);

class UQuest;
class AInteractable;

UCLASS(Blueprintable)
class AMech_RPGCharacter : public ACharacter {
	GENERATED_BODY()
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* TopDownCameraComponent;

	float energy;
	float health;
	float healthRegen;
	bool inCombat;
	bool channeling;
	bool isDead;
	static bool settingUpGroups;

	int32 id;
	int32 canAttack;
	int32 canMove;
	int32 canBeDamaged;

	static TArray<AMech_RPGCharacter*> characters;

	FTimerHandle TimerHandle_OutOfCombat;
	FTimerHandle TimerHandle_Invunrelbility;

	UPROPERTY()
		TArray<AWeapon*> weapons;

	UPROPERTY()
		AWeapon* currentWeapon = nullptr;

	UPROPERTY()
		UInventory* inventory = nullptr;

	UPROPERTY()
		UGroup* group = nullptr;

	UPROPERTY()
		AController* demandedController = nullptr;

	UPROPERTY()
		TArray<UAbility*> abilities;

	UPROPERTY()
		TArray<UArmour*> armour;

	UPROPERTY()
		TArray<UQuest*> quests;

	UPROPERTY()
		UAbility* currentAbility = nullptr;

	UPROPERTY()
		class UWidgetComponent* stats = nullptr;

	UPROPERTY()
		class UFloatingStats_BP* floatingStats = nullptr;
	class UCharacterStats* characterStats = nullptr;

	TSubclassOf<class UFloatingStats_BP> widgetClass = nullptr;
	TSubclassOf<class UFloatingTextUI> floatingTextClass = nullptr;
protected:
	virtual ~AMech_RPGCharacter();
public:
	AMech_RPGCharacter();

	virtual void SetActorHiddenInGame(bool bNewHidden) override;

	FORCEINLINE class UWidgetComponent* GetStats()
	{
		return stats;
	}

	class UFloatingStats_BP* GetFloatingStats() {
		return floatingStats;
	}

	void MirrorCamera(AMech_RPGCharacter* other)
	{
		CameraBoom->TargetArmLength = other->CameraBoom->TargetArmLength;
		CameraBoom->SetRelativeRotation(other->CameraBoom->RelativeRotation);
	}

	float GetTotalResistance(DamageEnums::DamageType damageType);

	UFUNCTION(BlueprintCallable, Category = "Quest")
		void AddQuest(UQuest* newQuest);

	UFUNCTION(BlueprintCallable, Category = "Quest")
		void AbandonQuest(UQuest* quest);

	UFUNCTION(BlueprintCallable, Category = "Combat")
		void SetInCombat(AMech_RPGCharacter* attacker, AMech_RPGCharacter* damagedMember);

	UFUNCTION(BlueprintCallable, Category = "Other")
		void Reset();

	UFUNCTION(BlueprintCallable, Category = "Other")
		void ResetInvunrelbility();

	UFUNCTION(BlueprintCallable, Category = "Other")
		void RemoveFromPlay();

	UFUNCTION(BlueprintCallable, Category = "Other")
		void PostHealthChange(FHealthChange healthChange);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
		AItem* AddItem(AItem* itemToAdd);

	UPROPERTY(BlueprintAssignable, Category = "Events")
		FPreHealthChangeEvent OnPreHealthChange;

	UPROPERTY(BlueprintAssignable, Category = "Events")
		FNPCInteractEvent OnNPCInteractEvent;

	UPROPERTY(BlueprintAssignable, Category = "Events")
		FItemPickUpEvent OnItemPickUpEvent;

	UPROPERTY(BlueprintAssignable, Category = "Events")
		FInteractEvent OnInteractEvent;

	UPROPERTY(BlueprintAssignable, Category = "Events")
		FPostHealthChangeEvent OnPostHealthChange;

	UPROPERTY(BlueprintAssignable, Category = "Events")
		FPostBeginPlay OnPostBeginPlay;

	UPROPERTY(BlueprintAssignable, Category = "Events")
		FStopFiring OnStopFiring;

	UPROPERTY(BlueprintAssignable, Category = "Events")
		FOutOfCombat OnOutOfCombat;

	UPROPERTY(BlueprintAssignable, Category = "Events")
		FSwappedWeapons OnSwappedWeapons;

	UPROPERTY(BlueprintAssignable, Category = "Events")
		FEnemyKilled OnEnemyKilled;

	UPROPERTY(BlueprintAssignable, Category = "Events")
		FQuestAdded OnQuestAdded;

	UFUNCTION(BlueprintCallable, Category = "Events")
		void EnemyKilled(AMech_RPGCharacter * character);

	UFUNCTION(BlueprintCallable, Category = "Events")
		void Interact(AInteractable * interactable);

	UFUNCTION(BlueprintCallable, Category = "Events")
		void NPCInteract(AMech_RPGCharacter * character);

	UFUNCTION(BlueprintCallable, Category = "Events")
		void ItemPickup(AItem* item);

	virtual void NotifyActorBeginCursorOver() override;

	virtual void OutOfCombat();

	FORCEINLINE bool GetInCombat()
	{
		return inCombat;
	}

	static const TArray<AMech_RPGCharacter*>& GetCharacters() {
		return characters;
	}

	class UCameraComponent* GetTopDownCamera() { return TopDownCameraComponent; };

	void SetTopDownCamera(class UCameraComponent* newCamera) {
		TopDownCameraComponent = newCamera;
	};

	UFUNCTION(BlueprintCallable, Category = "Quest")
		FORCEINLINE	TArray<UQuest*>& GetQuests()
	{
		return quests;
	}

	UFUNCTION(BlueprintCallable, Category = "Inventory")
		FORCEINLINE	UInventory * GetInventory() {
		return inventory;
	}

	void LookAt(AMech_RPGCharacter* other);

	UFUNCTION(BlueprintCallable, Category = "Ability")
		FORCEINLINE	bool Channelling() {
		return channeling;
	}

	UFUNCTION(BlueprintCallable, Category = "Ability")
		void SetChannelling(bool inChallenning) {
		channeling = inChallenning;
	}

	UFUNCTION(BlueprintCallable, Category = "Ability")
		FORCEINLINE	bool HasAbilities() {
		return GetAbilities().Num() > 0;
	}

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	void SetUpGroup();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loadout")
		int32 canUseAbilities;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loadout")
		FLoadout startingLoadout;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loadout")
		bool UseLoadout = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Role")
		TEnumAsByte<GroupEnums::Role> startingRole;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Group")
		TEnumAsByte<TeamEnums::Team> team;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		float healthChangeModifier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		float defenceModifier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		float maxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speed")
		float speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speed")
		float movementModifier;

	virtual	void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;

	UFUNCTION(BlueprintCallable, Category = "Armour")
		FORCEINLINE	TArray<UArmour*>& GetArmour() {
		return armour;
	}

	UFUNCTION(BlueprintCallable, Category = "Armour")
		UArmour* GetArmourByPosition(TEnumAsByte<ArmourEnums::ArmourPosition> pos);

	UFUNCTION(BlueprintCallable, Category = "CrowdControl")
		void ApplyCrowdControl(TEnumAsByte<EffectEnums::CrowdControl> controlModifications, bool positive);

	UFUNCTION(BlueprintCallable, Category = "Role")
		FORCEINLINE TEnumAsByte<GroupEnums::Role> GetRole()
	{
		return startingRole;
	}

	UFUNCTION(BlueprintCallable, Category = "Ability")
		FORCEINLINE UAbility* GetCurrentAbility() {
		return currentAbility;
	}

	UFUNCTION(BlueprintCallable, Category = "Ability")
		void SetCurrentAbility(UAbility* inAbility) {
		currentAbility = inAbility;
	}

	UFUNCTION(BlueprintCallable, Category = "Health")
		FORCEINLINE float GetHealth() {
		return health;
	}

	UFUNCTION(BlueprintCallable, Category = "Health")
		void SetHealth(float newVal) {
		health = newVal;
	}

	UFUNCTION(BlueprintCallable, Category = "Health")
		void ChangeHealth(FHealthChange healthChange);

	UFUNCTION(BlueprintCallable, Category = "Items")
		AItem* CalucluateItemDrop(UGroup * inGroup, ItemEnumns::ItemType type = ItemEnumns::Weapon);

	UFUNCTION(BlueprintCallable, Category = "Health")
		FORCEINLINE	bool IsDead() {
		return isDead;
	}

	UFUNCTION(BlueprintCallable, Category = "Health")
		virtual void SetDead(bool newVal);

	UFUNCTION(BlueprintCallable, Category = "Weapons")
		void AddWeapon(AWeapon* newWeapon);

	UFUNCTION(BlueprintCallable, Category = "Weapons")
		FORCEINLINE	TArray<AWeapon*>& GetWeapons() {
		return weapons;
	}

	UFUNCTION(BlueprintCallable, Category = "Weapons")
		void SetWeapons(TArray<AWeapon*> newVal) {
		weapons = newVal;
	}

	void SwapWeapon();

	UFUNCTION(BlueprintCallable, Category = "Weapons")
		FORCEINLINE AWeapon* GetCurrentWeapon() {
		return currentWeapon;
	}

	UFUNCTION(BlueprintCallable, Category = "Weapons")
		void SetCurrentWeapon(AWeapon* newVal);

	UFUNCTION(BlueprintCallable, Category = "Group")
		FORCEINLINE	UGroup* GetGroup() {
		return group;
	}

	UFUNCTION(BlueprintCallable, Category = "Group")
		void SetGroup(UGroup* newVal);

	bool CompareGroup(UGroup* inGroup);

	bool CompareGroup(AMech_RPGCharacter* inCharacter);

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Abilities")
		FORCEINLINE TArray<UAbility*>& GetAbilities() {
		return abilities;
	}

	UFUNCTION(BlueprintCallable, Category = "Abilities")
		void SetAbilities(TArray<UAbility*> newVal) {
		abilities = newVal;
	}

	UFUNCTION(BlueprintCallable, Category = "Abilities")
		void AddAbility(UAbility* newAbility);

	UFUNCTION(BlueprintCallable, Category = "Abilities")
		void RemoveAbility(UAbility* newAbility);

	UFUNCTION(BlueprintCallable, Category = "Role")
		virtual void CreatePresetRole(TEnumAsByte<GroupEnums::Role> inRole = GroupEnums::DPS, int32 grade = 1, int32 quaility = 0);

	UFUNCTION(BlueprintCallable, Category = "Role")
		void SetupWithLoadout();

	float GetModifierForDifficulty(TEnumAsByte<GameEnums::Difficulty> difficulty);

	FORCEINLINE AController* GetDemandedController() {
		return demandedController;
	}

	FORCEINLINE void SetDemandedController(AController* newVal) {
		demandedController = newVal;
	}

	UFUNCTION(BlueprintCallable, Category = "Health")
		FORCEINLINE	float GetMaxHealth() {
		return maxHealth;
	}

	UFUNCTION(BlueprintCallable, Category = "Health")
		void SetMaxHealth(float newVal) {
		maxHealth = newVal;
	}

	FORCEINLINE	int32 GetID() {
		return id;
	}

	FORCEINLINE void SetID(int32 newVal) {
		id = newVal;
	}

	UFUNCTION(BlueprintCallable, Category = "Health")
		FORCEINLINE	float GetHealthRegen() {
		return healthRegen;
	}

	UFUNCTION(BlueprintCallable, Category = "Health")
		void SetHealthRegen(float newVal) {
		healthRegen = newVal;
	}

	FORCEINLINE bool CanAttack() {
		return canAttack == 0;
	}

	FORCEINLINE bool CanMove() {
		return canMove == 0;
	}

	FORCEINLINE bool CanCast() {
		return canUseAbilities == 0;
	}

	FORCEINLINE int32& GetCanAttack() {
		return canAttack;
	}

	FORCEINLINE int32& GetCanMove() {
		return canMove;
	}

	FORCEINLINE float GetHealthChangeModifier() {
		return MAX(healthChangeModifier, 0.01F);
	}

	FORCEINLINE float GetDefenceModifier() {
		return MIN(defenceModifier, 0.99F);
	}

	FORCEINLINE void SetCanAttack(int32 newVal) {
		canAttack = newVal;
	}

	FORCEINLINE void SetCanMove(int32 newVal) {
		canMove = newVal;
	}

	FORCEINLINE void SetHealthChangeModifier(float newVal) {
		healthChangeModifier = newVal;
	}

	FORCEINLINE void SetDefenceModifier(float newVal) {
		defenceModifier = newVal;
	}

	FORCEINLINE bool GetCanBeDamaged() {
		return canBeDamaged == 0;
	}

	FORCEINLINE void SetCanBeDamaged(int32 newVal) {
		canBeDamaged = newVal;
	}

	FORCEINLINE float GetMovementModifier() {
		return movementModifier;
	}

	FORCEINLINE void SetMovementModifier(float newVal) {
		movementModifier = newVal;
	}

	FORCEINLINE float GetSpeed() {
		return speed;
	}

	FORCEINLINE void SetSpeed(float newVal) {
		speed = newVal;
	}
	FORCEINLINE class UCharacterStats* GetCharacterStats() const { return characterStats; }
	void SetCharacterStats(class UCharacterStats* val);
};