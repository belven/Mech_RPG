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
#include "Ability.h"
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
	/**
	 * Camera boom positioning the camera above the character
	 */
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
		USphereComponent* aoe = nullptr;

	UPROPERTY()
	class UWidgetComponent* stats = nullptr;

	TSubclassOf<class UFloatingStats_BP> widgetClass = nullptr;
	TSubclassOf<class UFloatingTextUI> floatingTextClass = nullptr;


protected:

	virtual ~AMech_RPGCharacter();

public:
	AMech_RPGCharacter();

	virtual void SetActorHiddenInGame(bool bNewHidden) override;

	class UWidgetComponent* GetStats();

	static const TArray<AMech_RPGCharacter*>& GetCharacters();

	class UCameraComponent* GetTopDownCamera() { return TopDownCameraComponent; };

	void SetTopDownCamera(class UCameraComponent* newCamera) {
		TopDownCameraComponent = newCamera;
	};

	void MirrorCamera(AMech_RPGCharacter* other)
	{
		CameraBoom->TargetArmLength = other->CameraBoom->TargetArmLength;
		CameraBoom->SetRelativeRotation(other->CameraBoom->RelativeRotation);
	}

	float GetTotalResistance(DamageEnums::DamageType damageType);

	UFUNCTION(BlueprintCallable, Category = "Quest")
		TArray<UQuest*>& GetQuests();

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

	UFUNCTION(BlueprintCallable, Category = "Inventory")
		UInventory* GetInventory(); 
	
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

	virtual void OutOfCombat();

	bool GetInCombat();

	void LookAt(AMech_RPGCharacter* other);

	UFUNCTION(BlueprintCallable, Category = "Ability")
		bool Channelling();

	UFUNCTION(BlueprintCallable, Category = "Ability")
		void SetChannelling(bool inChallenning);

	UFUNCTION(BlueprintCallable, Category = "Ability")
		bool HasAbilities();


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	void SetUpGroup();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loadout")
		int32 canUseAbilities;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loadout")
		FLoadout startingLoadout;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loadout")
		bool UseLoadout = false;


	UFUNCTION(BlueprintCallable, Category = "Role")
		TEnumAsByte<GroupEnums::Role> GetRole();

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
		TArray<UArmour*>& GetArmour();

	UFUNCTION(BlueprintCallable, Category = "Armour")
		UArmour* GetArmourByPosition(TEnumAsByte<ArmourEnums::ArmourPosition> pos);

	UFUNCTION(BlueprintCallable, Category = "CrowdControl")
		void ApplyCrowdControl(TEnumAsByte<EffectEnums::CrowdControl> controlModifications, bool positive);

	UFUNCTION(BlueprintCallable, Category = "Ability")
		UAbility* GetCurrentAbility();

	UFUNCTION(BlueprintCallable, Category = "Ability")
		void SetCurrentAbility(UAbility* inAbility);

	UFUNCTION(BlueprintCallable, Category = "ID")
		float GetEnergy();

	UFUNCTION(BlueprintCallable, Category = "ID")
		float GetHealth();

	UFUNCTION(BlueprintCallable, Category = "Group")
		void SetEnergy(float newVal);

	UFUNCTION(BlueprintCallable, Category = "Group")
		void SetHealth(float newVal);

	void ChangeHealth(FHealthChange healthChange);


	UFUNCTION(BlueprintCallable, Category = "Weapons")
		bool IsDead();

	UFUNCTION(BlueprintCallable, Category = "Weapons")
		virtual void SetDead(bool newVal);


	UFUNCTION(BlueprintCallable, Category = "Weapons")
		void AddWeapon(AWeapon* newWeapon);

	UFUNCTION(BlueprintCallable, Category = "Weapons")
		TArray<AWeapon*>& GetWeapons();

	UFUNCTION(BlueprintCallable, Category = "Weapons")
		void SetWeapons(TArray<AWeapon*> newVal);

	UFUNCTION(BlueprintCallable, Category = "Health")
		AWeapon* GetCurrentWeapon();

	UFUNCTION(BlueprintCallable, Category = "Health")
		void SetCurrentWeapon(AWeapon* newVal);

	UFUNCTION(BlueprintCallable, Category = "Health")
		UGroup* GetGroup();

	UFUNCTION(BlueprintCallable, Category = "Health")
		void SetGroup(UGroup* newVal);

	UFUNCTION(BlueprintCallable, Category = "Health")
		int32 GetID();

	UFUNCTION(BlueprintCallable, Category = "Health")
		void SetID(int32 newVal);

	UFUNCTION(BlueprintCallable, Category = "Health")
		float GetHealthRegen();

	UFUNCTION(BlueprintCallable, Category = "Health")
		void SetHealthRegen(float newVal);

	bool CompareGroup(UGroup* inGroup);

	bool CompareGroup(AMech_RPGCharacter* inCharacter);

	AController* GetDemandedController();
	void SetDemandedController(AController* newVal);

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Abilities")
		TArray<UAbility*>& GetAbilities();

	UFUNCTION(BlueprintCallable, Category = "Abilities")
		void SetAbilities(TArray<UAbility*> newVal);

	UFUNCTION(BlueprintCallable, Category = "Abilities")
		void AddAbility(UAbility* newAbility);

	void SwapWeapon();

	USphereComponent* GetAOE();
	void SetAOE(USphereComponent* newVal);

	UFUNCTION(BlueprintCallable, Category = "Health")
		float GetMaxHealth();
	void SetMaxHealth(float newVal);

	bool CanAttack();
	bool CanMove();
	bool CanCast();
	int32& GetCanAttack();
	int32& GetCanMove();

	float GetHealthChangeModifier();
	float GetDefenceModifier();

	void SetCanAttack(int32 newVal);
	void SetCanMove(int32 newVal);

	void SetHealthChangeModifier(float newVal);
	void SetDefenceModifier(float newVal);

	UFUNCTION(BlueprintCallable, Category = "Role")
		virtual void CreatePresetRole(TEnumAsByte<GroupEnums::Role> inRole = GroupEnums::DPS, int32 grade = 0, int32 quaility = 0);

	UFUNCTION(BlueprintCallable, Category = "Role")
		void SetupWithLoadout();

	bool GetCanBeDamaged();
	void SetCanBeDamaged(int32 newVal);

	float GetMovementModifier();
	float GetSpeed();

	void SetMovementModifier(float newVal);
	void SetSpeed(float newVal);

	float GetModifierForDifficulty(TEnumAsByte<GameEnums::Difficulty> difficulty);
};

