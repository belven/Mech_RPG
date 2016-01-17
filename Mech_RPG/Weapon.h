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
		float damage = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		float critChance = 15;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		float range = 1000;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		float fireRate = 0.5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		bool heals = false;
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
	AWeapon();
	float GetDamage();
	float GetRange();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Particles)
		UParticleSystemComponent* partclSystem;

	void SetDamage(float newVal);
	void SetRange(float newVal);

	virtual bool CanFire();
	virtual void Fire(class AMech_RPGCharacter* targetr);
	virtual void Fire(class ACover* target);	
	
	virtual	void SetOwner(AMech_RPGCharacter* inOwner) override;

	UFUNCTION(BlueprintCallable, Category = "Weapon")
		virtual void StopFire();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
		static AWeapon* CreateWeapon(AMech_RPGCharacter* inOwner, FWeaponParams inSettings);

	float GetFireRate();
	void SetFireRate(float newVal);

	float GetCritChance();
	void SetCritChance(float newVal);

	virtual void Tick(float DeltaTime);
	bool Heals();
	void SetHeals(bool newVal);

	DamageEnums::DamageType GetDamageType();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
		virtual float GetProgressBarPercent();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
		static AWeapon* CreatePresetWeapon(AMech_RPGCharacter* inOwner, TEnumAsByte<WeaponEnums::WeaponType> type = WeaponEnums::SMG);
};
