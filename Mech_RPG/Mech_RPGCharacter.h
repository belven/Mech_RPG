// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "Tag.h"
#include "Weapon.h"
#include "Group.h"
#include "Array.h"
#include "Mech_RPGCharacter.generated.h"

UCLASS(Blueprintable)
class AMech_RPGCharacter : public ACharacter
{
	GENERATED_BODY()
private:

	/**
	 * Camera boom positioning the camera above the character
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/**
	 * Top down camera
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	bool isDead;
	float energy;
	float health;
	int32 id;


	TArray<AWeapon*> weapons;
	AWeapon* currentWeapon;

	UGroup* group;
	USphereComponent* aoeDecetion;
public:
	AMech_RPGCharacter();
	virtual	void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;

	UFUNCTION(BlueprintCallable , Category  = "ID")
		float GetEnergy();

	UFUNCTION(BlueprintCallable , Category  = "ID")
		float GetHealth();

	UFUNCTION(BlueprintCallable , Category  = "Group")
		void SetEnergy(float newVal);

	UFUNCTION(BlueprintCallable , Category  = "Group")
		void SetHealth(float newVal);

	void Hit(AMech_RPGCharacter* other, float damage);

	UFUNCTION(BlueprintCallable , Category  = "Weapons")
		bool IsDead();

	UFUNCTION(BlueprintCallable , Category  = "Weapons")
		void SetDead(bool newVal);



	UFUNCTION(BlueprintCallable, Category = "Weapons")
		void AddWeapon(AWeapon* newWeapon);

	UFUNCTION(BlueprintCallable, Category = "Weapons")
		TArray<AWeapon*> GetWeapons();

	UFUNCTION(BlueprintCallable, Category = "Weapons")
		void SetWeapons(TArray<AWeapon*> newVal);

	UFUNCTION(BlueprintCallable , Category  = "Health")
		AWeapon* GetCurrentWeapon();

	UFUNCTION(BlueprintCallable , Category  = "Health")
		void SetCurrentWeapon(AWeapon* newVal);



	UFUNCTION(BlueprintCallable , Category  = "Health")
		UGroup* GetGroup();

	UFUNCTION(BlueprintCallable , Category  = "Health")
		void SetGroup(UGroup* newVal);

	UFUNCTION(BlueprintCallable , Category  = "Health")
		int32 GetID();

	UFUNCTION(BlueprintCallable , Category  = "Health")
		void SetID(int32 newVal);
	bool CompareGroup(UGroup* inGroup);
	bool CompareGroup(AMech_RPGCharacter* inCharacter);
};

