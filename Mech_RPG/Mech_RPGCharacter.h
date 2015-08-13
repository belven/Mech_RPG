// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "Tag.h"
#include "Weapon.h"
#include "Array.h"
#include "Mech_RPGCharacter.generated.h"

UCLASS(Blueprintable)
class AMech_RPGCharacter : public ACharacter
{
	GENERATED_BODY()

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
private:
	float energy;
	float health;
	TArray<AWeapon*> weapons;
	bool isDead;

public:
	AMech_RPGCharacter();

	float GetEnergy();
	float GetHealth();

	void SetEnergy(float newVal);
	void SetHealth(float newVal);

	void Hit(AMech_RPGCharacter* other, float damage, TArray<FTag>* tags);

	virtual void PossessedBy(AController* NewController) override;
	void AddWeapon(AWeapon* newWeapon);

	TArray<AWeapon*> GetWeapons();
	void SetWeapons(TArray<AWeapon*> newVal);

	virtual	void BeginPlay() override;
	bool IsDead();
	void SetDead(bool newVal);

};

