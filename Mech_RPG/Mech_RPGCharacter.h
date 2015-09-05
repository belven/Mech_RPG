// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "Tag.h"
#include "Weapon.h"
#include "Group.h"
#include "Array.h"
#include "Ability.h"
#include "Mech_RPGCharacter.generated.h"

UCLASS(Blueprintable)
class AMech_RPGCharacter : public ACharacter {
	GENERATED_BODY()
private:
	/**
	 * Top down camera
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/**
	 * Camera boom positioning the camera above the character
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	bool isDead;
	float energy;
	float health;
	float healthRegen;
	float maxHealth;

	int32 id;
	int32 canAttack;
	int32 canMove;

	TArray<AWeapon*> weapons;
	AWeapon* currentWeapon;

	UGroup* group;
	AController* demandedController;

	TArray<UAbility*> abilities;
	USphereComponent* aoe;
public:
	AMech_RPGCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Role")
		TEnumAsByte<GroupEnums::Role> startingRole;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Group")
		int32 startingGroupID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		float damageModifier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		float defenceModifier;

	virtual	void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;

	UFUNCTION(BlueprintCallable, Category = "Health")
		float GetEnergy();

	UFUNCTION(BlueprintCallable, Category = "Health")
		float GetHealth();

	UFUNCTION(BlueprintCallable, Category = "Health")
		void SetEnergy(float newVal);

	UFUNCTION(BlueprintCallable, Category = "Health")
		void SetHealth(float newVal);

	void Hit(AMech_RPGCharacter* other, float damage);

	UFUNCTION(BlueprintCallable, Category = "Health")
		bool IsDead();

	UFUNCTION(BlueprintCallable, Category = "Health")
		void SetDead(bool newVal);


	UFUNCTION(BlueprintCallable, Category = "Health")
		void AddWeapon(AWeapon* newWeapon);

	UFUNCTION(BlueprintCallable, Category = "Health")
		TArray<AWeapon*> GetWeapons();

	UFUNCTION(BlueprintCallable, Category = "Weapons")
		void SetWeapons(TArray<AWeapon*> newVal);

	UFUNCTION(BlueprintCallable, Category = "Weapons")
		AWeapon* GetCurrentWeapon();

	UFUNCTION(BlueprintCallable, Category = "Weapons")
		void SetCurrentWeapon(AWeapon* newVal);


	UFUNCTION(BlueprintCallable, Category = "Weapons")
		UGroup* GetGroup();

	UFUNCTION(BlueprintCallable, Category = "Weapons")
		void SetGroup(UGroup* newVal);

	UFUNCTION(BlueprintCallable, Category = "Group")
		int32 GetID();

	UFUNCTION(BlueprintCallable, Category = "Group")
		void SetID(int32 newVal);

	UFUNCTION(BlueprintCallable, Category = "ID")
		float GetHealthRegen();

	UFUNCTION(BlueprintCallable, Category = "ID")
		void SetHealthRegen(float newVal);

	bool CompareGroup(UGroup* inGroup);

	bool CompareGroup(AMech_RPGCharacter* inCharacter);

	AController* GetDemandedController();
	void SetDemandedController(AController* newVal);

	virtual void Tick(float DeltaTime) override;

	TArray<UAbility*> GetAbilities();
	void SetAbilities(TArray<UAbility*> newVal);

	void SwapWeapon();

	USphereComponent* GetAOE();
	void SetAOE(USphereComponent* newVal);

	float GetMaxHealth();
	void SetMaxHealth(float newVal);

	bool CanAttack();
	bool CanMove();

	float GetDamageModifier();
	float GetDefenceModifier();

	void SetCanAttack(int32 newVal);
	void SetCanMove(int32 newVal);

	void SetDamageModifier(float newVal);
	void SetDefenceModifier(float newVal);

	void CreatePresetRole(TEnumAsByte<GroupEnums::Role> role = GroupEnums::DPS);
};

