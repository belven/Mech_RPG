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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

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



	UFUNCTION(BlueprintCallable, Category = "Weapons")
		void AddWeapon(AWeapon* newWeapon);

	UFUNCTION(BlueprintCallable, Category = "Weapons")
		TArray<AWeapon*> GetWeapons();

	UFUNCTION(BlueprintCallable, Category = "Weapons")
		void SetWeapons(TArray<AWeapon*> newVal);

	UFUNCTION(BlueprintCallable, Category = "Weapons")
		AWeapon* GetCurrentWeapon();

	UFUNCTION(BlueprintCallable, Category = "Weapons")
		void SetCurrentWeapon(AWeapon* newVal);



	UFUNCTION(BlueprintCallable, Category = "Group")
		UGroup* GetGroup();

	UFUNCTION(BlueprintCallable, Category = "Group")
		void SetGroup(UGroup* newVal);

	UFUNCTION(BlueprintCallable, Category = "ID")
		int32 GetID();

	UFUNCTION(BlueprintCallable, Category = "ID")
		void SetID(int32 newVal);
};

