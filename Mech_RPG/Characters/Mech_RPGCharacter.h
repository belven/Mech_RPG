UENUM(BlueprintType)
namespace TeamEnums
{
	enum Team
	{
		Paladins,
		Mercenaries
	};
}

UENUM(BlueprintType)
namespace EffectEnums
{
	enum CrowdControl
	{
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
#include "Group.h"
#include "Array.h"
#include "UI/FloatingStats_BP.h"
#include "Delegates/DelegateCombinations.h"
#include "EventManager.h"
#include "Mech_RPGCharacter.generated.h"

#define MIN(a,b) (a < b) ? (a) : (b)
#define MAX(a,b) (a > b) ? (a) : (b)
#define mCreatePresetWeapon(type, grade, quailty) UWeapon::CreatePresetWeapon(this, type, grade, quailty)
#define mCreatePresetAbility(type) UAbility::CreatePresetAbility(this,type)
#define mCreateChannelledAbility(ability, Duration, loc, trace) UChannelledAbility::CreateChannelledAbility(this, ability, Duration, loc, trace)
#define mCreatePresetRole(role) AMech_RPGCharacter::CreatePresetRole(role)
#define mGetDefaultArmourValue(grade) UArmour::GetDeafultValue(grade)
#define mCreateTimedHealthChange(changeAmount, cooldown, duration, rate, heals) UTimedHealthChange::CreateTimedHealthChange(this, cooldown, changeAmount, rate, duration, heals)

#define ArmourMap TPair<EArmourPosition, class UArmour*>

USTRUCT(BlueprintType)
struct FLoadout
{
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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPostBeginPlay, AMech_RPGCharacter*, character);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEnemyKilled, AMech_RPGCharacter*, character);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNPCInteractEvent, AMech_RPGCharacter*, character);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FItemPickUpEvent, UItem*, item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInteractEvent, AInteractable*, interactable);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FQuestAdded, UQuest*, quest);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStopFiring);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOutOfCombat);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSwappedWeapons, UWeapon*, oldWeapon, UWeapon*, newWeapon);

class UQuest;
class AInteractable;
class USkillTree;
class UAbility;
class UInventory;
class UWeapon;

UCLASS(Blueprintable)
class AMech_RPGCharacter : public ACharacter
{
	GENERATED_BODY()
protected:
	static const float lowHealth;
	static const float mediumHealth;
	static const float highHealth;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* TopDownCameraComponent;

	float energy;
	float health;
	float healthRegen;
	bool inCombat;
	bool channeling;
	bool isDead;
	bool isAlly;

	static bool settingUpGroups;

	int32 id;
	int32 canAttack;
	int32 canMove;
	int32 canBeDamaged;

	FTimerHandle TimerHandle_OutOfCombat;
	FTimerHandle TimerHandle_Invunrelbility;

	UPROPERTY(EditAnywhere, Category = "Loadout")
		int32 canUseAbilities;

	UPROPERTY(EditAnywhere, Category = "Loadout")
		FLoadout startingLoadout;

	UPROPERTY(EditAnywhere, Category = "Loadout")
		bool UseLoadout = false;

	UPROPERTY(EditAnywhere, Category = "Role")
		ERole startingRole;

	UPROPERTY(EditAnywhere, Category = "Group")
		TEnumAsByte<TeamEnums::Team> team;

	UPROPERTY(EditAnywhere, Category = "Damage")
		float healthChangeModifier;

	UPROPERTY(EditAnywhere, Category = "Damage")
		float defenceModifier;

	UPROPERTY(EditAnywhere, Category = "Damage")
		float maxHealth;

	UPROPERTY(EditAnywhere, Category = "Speed")
		float speed;

	UPROPERTY(EditAnywhere, Category = "Speed")
		float speedModifier;

	UPROPERTY(EditAnywhere, Category = "Speed")
		float critChanceModifier;

	UPROPERTY(EditAnywhere, Category = "Speed")
		float attackSpeedModifier;

	UPROPERTY()
		UWeapon* currentWeapon = nullptr;

	UPROPERTY()
		UInventory* inventory = nullptr;

	UPROPERTY()
		UGroup* group = nullptr;

	UPROPERTY()
		AController* demandedController = nullptr;

	UPROPERTY()
		TArray<UAbility*> abilities;

	UPROPERTY()
		TMap<EArmourPosition, class UArmour*> armour;

	UPROPERTY()
		TArray<UQuest*> quests;

	UPROPERTY()
		TArray<USkillTree*> skillTrees;

	UPROPERTY()
		UAbility* currentAbility = nullptr;

	UPROPERTY()
		class UWidgetComponent* stats = nullptr;

	UPROPERTY()
		class UFloatingStats_BP* floatingStats = nullptr;

	UPROPERTY()
		class UCharacterStats* characterStats = nullptr;

	TSubclassOf<class UFloatingStats_BP> widgetClass = nullptr;
	TSubclassOf<class UFloatingTextUI> floatingTextClass = nullptr;

	USphereComponent* radiusDection;
	void CreateSkillTrees();
protected:
	virtual ~AMech_RPGCharacter();
public:
	AMech_RPGCharacter();
	bool isPlayer;

	virtual void SetActorHiddenInGame(bool bNewHidden) override;
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

	FORCEINLINE USphereComponent* GetRadiusDection()
	{
		return radiusDection;
	}

	FORCEINLINE class UWidgetComponent* GetStats()
	{
		return stats;
	}

	class UFloatingStats_BP* GetFloatingStats()
	{
		return floatingStats;
	}

	void MirrorCamera(AMech_RPGCharacter* other)
	{
		CameraBoom->TargetArmLength = other->CameraBoom->TargetArmLength;
		CameraBoom->SetRelativeRotation(other->CameraBoom->RelativeRotation);
	}

	UFUNCTION(BlueprintCallable, Category = "Skill Tree")
		float GetStatBonus(EStatEnum statType);

	bool IsAlly(AMech_RPGCharacter* other);
	bool IsEnemy(AMech_RPGCharacter* other);

	float GetTotalResistance(EDamageType damageType);

	UFUNCTION(BlueprintCallable, Category = "Armour")
		void CreateArmour(float phsyicalResistance, float blastResistance, float energyResistance, int32 grade, int32 quaility);

	UFUNCTION(BlueprintCallable, Category = "Quest")
		void AddQuest(UQuest* newQuest);

	UFUNCTION(BlueprintCallable, Category = "Quest")
		void RemoveQuest(UQuest* quest);

	UFUNCTION(BlueprintCallable, Category = "Combat")
		void SetInCombat(AMech_RPGCharacter* attacker = nullptr, AMech_RPGCharacter* damagedMember = nullptr);

	UFUNCTION(BlueprintCallable, Category = "Other")
		void ResetCharacter();

	void MaximiseHealth();

	void RemoveFromPlay();

	UFUNCTION(BlueprintCallable, Category = "Other")
		void PostHealthChange(FHealthChange healthChange);

	void CharacterDied(FHealthChange &healthChange);

	void UpdateStatsUI();

	void SpawnItem(AMech_RPGCharacter* character);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
		UItem* AddItem(UItem* itemToAdd);

	UPROPERTY()
		FPreHealthChangeEvent OnPreHealthChange;

	UPROPERTY()
		FPostHealthChangeEvent OnPostHealthChange;

	UPROPERTY()
		FEnemyKilled OnEnemyKilled;

	UPROPERTY(BlueprintAssignable, Category = "Events")
		FNPCInteractEvent OnNPCInteractEvent;

	UPROPERTY(BlueprintAssignable, Category = "Events")
		FItemPickUpEvent OnItemPickUpEvent;

	UPROPERTY(BlueprintAssignable, Category = "Events")
		FInteractEvent OnInteractEvent;

	UPROPERTY(BlueprintAssignable, Category = "Events")
		FPostBeginPlay OnPostBeginPlay;

	UPROPERTY(BlueprintAssignable, Category = "Events")
		FStopFiring OnStopFiring;

	UPROPERTY(BlueprintAssignable, Category = "Events")
		FOutOfCombat OnOutOfCombat;

	UPROPERTY(BlueprintAssignable, Category = "Events")
		FSwappedWeapons OnSwappedWeapons;

	UPROPERTY(BlueprintAssignable, Category = "Events")
		FQuestAdded OnQuestAdded;

	UFUNCTION(BlueprintCallable, Category = "Events")
		void EnemyKilled(AMech_RPGCharacter * character);

	UFUNCTION(BlueprintCallable, Category = "Events")
		void Interact(AInteractable * interactable);

	UFUNCTION(BlueprintCallable, Category = "Events")
		void NPCInteract(AMech_RPGCharacter * character);

	virtual void NotifyActorBeginCursorOver() override;

	virtual void OutOfCombat();

	void SetRemoveFromPlayTimer();

	void Resurrect();

	void FindSpawnpoint();

	FORCEINLINE bool IsInCombat()
	{
		return inCombat;
	}

	class UCameraComponent* GetTopDownCamera() { return TopDownCameraComponent; };

	void SetTopDownCamera(class UCameraComponent* newCamera)
	{
		TopDownCameraComponent = newCamera;
	};

	UFUNCTION(BlueprintCallable, Category = "Quest")
		FORCEINLINE	TArray<UQuest*>& GetQuests()
	{
		return quests;
	}

	UFUNCTION(BlueprintCallable, Category = "Inventory")
		FORCEINLINE	UInventory * GetInventory()
	{
		return inventory;
	}

	void LookAt(AMech_RPGCharacter* other);

	UFUNCTION(BlueprintCallable, Category = "Ability")
		FORCEINLINE	bool Channelling()
	{
		return channeling;
	}

	UFUNCTION(BlueprintCallable, Category = "Ability")
		void SetChannelling(bool inChallenning)
	{
		channeling = inChallenning;
	}

	UFUNCTION(BlueprintCallable, Category = "Ability")
		FORCEINLINE	bool HasAbilities()
	{
		return GetAbilities().Num() > 0;
	}

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	void SetUpGroup();

	virtual	void BeginPlay() override;

	void SetUpWidgets();

	UFUNCTION(BlueprintCallable, Category = "Armour")
		FORCEINLINE	TArray<UArmour*> GetArmourList()
	{
		TArray<UArmour*> armourList;
		armour.GenerateValueArray(armourList);
		return armourList;
	}

	FORCEINLINE	TMap<EArmourPosition, class UArmour*>& GetArmour()
	{
		return armour;
	}

	UFUNCTION(BlueprintCallable, Category = "Armour")
		UArmour* GetArmourByPosition(EArmourPosition pos);

	// if remove is false then it will apply the effect, if it's true it will remove it
	UFUNCTION(BlueprintCallable, Category = "CrowdControl")
		void ApplyCrowdControl(TEnumAsByte<EffectEnums::CrowdControl> controlModifications, bool remove);

	UFUNCTION(BlueprintCallable, Category = "Role")
		FORCEINLINE ERole GetRole()
	{
		return StartingRole();
	}

	UFUNCTION(BlueprintCallable, Category = "Ability")
		FORCEINLINE UAbility* GetCurrentAbility()
	{
		return currentAbility;
	}

	UFUNCTION(BlueprintCallable, Category = "Ability")
		void SetCurrentAbility(UAbility* inAbility)
	{
		currentAbility = inAbility;
	}

	UFUNCTION(BlueprintCallable, Category = "Health")
		FORCEINLINE float GetHealth()
	{
		return health;
	}

	UFUNCTION(BlueprintCallable, Category = "Health")
		void SetHealth(float newVal)
	{
		health = newVal;
	}

	UFUNCTION(BlueprintCallable, Category = "Health")
		virtual void ChangeHealth(FHealthChange healthChange);

	UFUNCTION(BlueprintCallable, Category = "Items")
		UItem* CalucluateItemDrop(UGroup * inGroup, ItemEnumns::ItemType type = ItemEnumns::Weapon);

	UFUNCTION(BlueprintCallable, Category = "Health")
		FORCEINLINE	bool IsDead()
	{
		return isDead;
	}

	UFUNCTION(BlueprintCallable, Category = "Health")
		virtual void SetDead(bool newVal);

	UFUNCTION(BlueprintCallable, Category = "Weapons")
		FORCEINLINE UWeapon* GetCurrentWeapon()
	{
		return currentWeapon;
	}

	UFUNCTION(BlueprintCallable, Category = "Weapons")
		void SetCurrentWeapon(UWeapon* newVal);

	UFUNCTION(BlueprintCallable, Category = "Group")
		FORCEINLINE	UGroup* GetGroup()
	{
		return group;
	}

	UFUNCTION(BlueprintCallable, Category = "Group")
		void SetGroup(UGroup* newVal);

	void CleanExistingGroup();

	// Compares the characters group with another, true if allies, false if enemies
	bool CompareGroup(UGroup* inGroup);

	// Compares the characters group with another, true if allies, false if enemies
	bool CompareGroup(AMech_RPGCharacter* inCharacter);

	virtual void Tick(float DeltaTime) override;

	void UpdateHealthBarRot();

	UFUNCTION(BlueprintCallable, Category = "Abilities")
		FORCEINLINE TArray<UAbility*>& GetAbilities()
	{
		return abilities;
	}

	UFUNCTION(BlueprintCallable, Category = "Abilities")
		void SetAbilities(TArray<UAbility*> newVal)
	{
		abilities = newVal;
	}

	UFUNCTION(BlueprintCallable, Category = "Abilities")
		void AddAbility(UAbility* newAbility);

	UFUNCTION(BlueprintCallable, Category = "Abilities")
		void RemoveAbility(UAbility* newAbility);

	UFUNCTION(BlueprintCallable, Category = "Role")
		virtual void CreatePresetRole(ERole inRole = ERole::DPS, int32 grade = 1, int32 quaility = 0);

	UFUNCTION(BlueprintCallable, Category = "Role")
		void SetupWithLoadout();

	FORCEINLINE AController* GetDemandedController()
	{
		return demandedController;
	}

	void SetDemandedController(AController* newVal)
	{
		demandedController = newVal;
	}

	UFUNCTION(BlueprintCallable, Category = "Health")
		float GetMaxHealth();

	UFUNCTION(BlueprintCallable, Category = "Health")
		void SetMaxHealth(float newVal)
	{
		maxHealth = newVal;
	}

	FORCEINLINE	int32 GetID()
	{
		return id;
	}

	void SetID(int32 newVal)
	{
		id = newVal;
	}

	UFUNCTION(BlueprintCallable, Category = "Health")
		FORCEINLINE	float GetHealthRegen()
	{
		return healthRegen;
	}

	UFUNCTION(BlueprintCallable, Category = "Health")
		void SetHealthRegen(float newVal)
	{
		healthRegen = newVal;
	}

	FORCEINLINE bool CanAttack()
	{
		return canAttack < 1;
	}

	FORCEINLINE bool CanMove()
	{
		return canMove < 1;
	}

	FORCEINLINE bool CanCast()
	{
		return canUseAbilities < 1;
	}

	FORCEINLINE int32& GetCanAttack()
	{
		return canAttack;
	}

	FORCEINLINE int32& GetCanMove()
	{
		return canMove;
	}

	FORCEINLINE float GetDefenceModifier()
	{
		return MIN(defenceModifier, 0.99F);
	}

	void SetCanAttack(int32 newVal)
	{
		canAttack = newVal;
	}

	void SetCanMove(int32 newVal)
	{
		canMove = newVal;
	}

	void SetHealthChangeModifier(float newVal)
	{
		healthChangeModifier = newVal;
	}

	float GetHealthChangeModifier();

	void SetDefenceModifier(float newVal)
	{
		defenceModifier = newVal;
	}

	FORCEINLINE bool GetCanBeDamaged()
	{
		return canBeDamaged < 1;
	}

	void SetCanBeDamaged(int32 newVal)
	{
		canBeDamaged = newVal;
	}

	FORCEINLINE float GetSpeedModifier()
	{
		return speedModifier;
	}

	void SetSpeedModifier(float newVal)
	{
		speedModifier = newVal;
	}

	FORCEINLINE float GetSpeed()
	{
		return speed;
	}

	void SetSpeed(float newVal)
	{
		speed = newVal;
	}

	FORCEINLINE class UCharacterStats* GetCharacterStats() const { return characterStats; }

	void SetCharacterStats(class UCharacterStats* val);

	FORCEINLINE TEnumAsByte<TeamEnums::Team> GetTeam() const { return team; }

	void SetTeam(TEnumAsByte<TeamEnums::Team> val) { team = val; }

	FORCEINLINE ERole StartingRole() const { return startingRole; }

	void StartingRole(ERole val) { startingRole = val; }

	UFUNCTION(BlueprintCallable, Category = "Attack Speed")
		float GetAttackSpeedModifier();

	UFUNCTION(BlueprintCallable, Category = "Attack Speed")
		void SetAttackSpeedModifier(float val) { attackSpeedModifier = val; }

	UFUNCTION(BlueprintCallable, Category = "Crit Chance")
		FORCEINLINE float GetCritChanceModifier() { return critChanceModifier; }

	UFUNCTION(BlueprintCallable, Category = "Crit Chance")
		void SetCritChanceModifier(float val) { critChanceModifier = val; }

	UFUNCTION(BlueprintCallable, Category = "Skill Tree")
		TArray<USkillTree *>& GetSkillTrees() { return skillTrees; }

	UFUNCTION(BlueprintCallable, Category = "Skill Tree")
		USkillTree * GetSkillTreeBySpec(ESpecialisation spec);

	UFUNCTION(BlueprintCallable, Category = "Skill Tree")
		void SetSkillTrees(TArray<USkillTree *> val) { skillTrees = val; }
	void ChangedOthersHealth(FHealthChange healthChange);
};