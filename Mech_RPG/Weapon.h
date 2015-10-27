// Fill out your copyright notice in the Description page of Project Settings.


UENUM(BlueprintType)
namespace WeaponEnums {
	enum WeaponType {
		Sniper,
		Bio_Repair,
		SMG,
		Shotgun,
		RPG
	};
}

#pragma once
#include "MechAttachment.h"
#include "Armour.h"
#include "Weapon.generated.h"


USTRUCT(BlueprintType)
struct FWeaponParams {
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		float damage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		float range;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		float fireRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		bool heals;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		TEnumAsByte<DamageEnums::DamageType> damageType = DamageEnums::Physical;
};

UCLASS(Blueprintable)
class MECH_RPG_API AWeapon : public AMechAttachment {
	GENERATED_BODY()
protected:
	FWeaponParams settings;
	bool canFire;
	float lastTime;

public:
	float GetDamage();
	float GetRange();

	void SetDamage(float newVal);
	void SetRange(float newVal);

	virtual bool CanFire();
	virtual void Fire(class AMech_RPGCharacter* target, AMech_RPGCharacter* owner);
	virtual void Fire(class ACover* target, AMech_RPGCharacter* owner);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
		static AWeapon* CreateWeapon(AActor* inOwner, FWeaponParams inSettings);

	float GetFireRate();
	void SetFireRate(float newVal);

	virtual void Tick(float DeltaTime);
	bool Heals();
	void SetHeals(bool newVal);

	DamageEnums::DamageType GetDamageType();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
		virtual float GetProgressBarPercent();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
		static AWeapon* CreatePresetWeapon(AMech_RPGCharacter* inOwner, TEnumAsByte<WeaponEnums::WeaponType> type = WeaponEnums::SMG);
};
