// Fill out your copyright notice in the Description page of Project Settings.


UENUM(BlueprintType)
namespace WeaponEnums {
	enum WeaponType {
		Sniper,
		Bio_Repair,
		SMG,
		Shotgun,
		RPG,
		Sword,
		End
	};
}



#pragma once

#include "Items/Equipable.h"
#include "Items/Armour.h"
#include "MiscLibrary.h"
#include "Weapon.generated.h"


USTRUCT(BlueprintType)
struct FWeaponParams {
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		float healthChange = 100;
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		TEnumAsByte<QualityEnums::Quality> quality = QualityEnums::Base;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		int32 grade = 0;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFired);

UCLASS(Blueprintable)
class MECH_RPG_API AWeapon : public AEquipable {
	GENERATED_BODY()
protected:
	FWeaponParams settings;
	bool canFire = true;
	float lastTime = 0;
	UAudioComponent* audioComp = nullptr;

public:
	AWeapon();
	float GetChangeAmount();
	float GetRange(); 
	
	UFUNCTION(BlueprintCallable, Category = "Item")
		virtual AItem* Copy();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Particles)
		UParticleSystemComponent* partclSystem;

	void SetChangeAmount(float newVal);
	void SetRange(float newVal);

	virtual bool CanFire();
	virtual void Fire(class AMech_RPGCharacter* targetr);
	virtual void Fire(class ACover* target);

	virtual	void SetOwner(AMech_RPGCharacter* inOwner) override;

	UFUNCTION(BlueprintCallable, Category = "Weapon")
		virtual void StopFire();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
		static AWeapon* CreateWeapon(UWorld* world, AMech_RPGCharacter* inOwner, FWeaponParams inSettings);

	UPROPERTY(BlueprintAssignable, Category = "Events")
		FFired OnFire;

	float GetFireRate();
	void SetFireRate(float newVal);

	float GetCritChance();
	void SetCritChance(float newVal);

	virtual void Tick(float DeltaTime) override;
	bool Heals();
	void SetHeals(bool newVal);

	void SetSettings(FWeaponParams newSettings);

	DamageEnums::DamageType GetChangeAmountType();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
		virtual float GetProgressBarPercent();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
		virtual FString GetTooltipText() override;

	UFUNCTION(BlueprintCallable, Category = "Weapon")
		static AWeapon* CreatePresetWeapon(UWorld* world, AMech_RPGCharacter* inOwner, TEnumAsByte<WeaponEnums::WeaponType> weaponType = WeaponEnums::SMG, int32 weaponGrade = 0, int32 weaponQuality = 0);
};
