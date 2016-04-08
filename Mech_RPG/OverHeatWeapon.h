// Copyright of Explosive Industries

#pragma once

#include "Weapon.h"
#include "OverHeatWeapon.generated.h"

USTRUCT(BlueprintType)
struct FOverheatWeaponParams : public FWeaponParams {
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		float heatGenerated;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		float heatLosePerTick;
};

UCLASS()
class MECH_RPG_API AOverHeatWeapon : public AWeapon {
	GENERATED_BODY()

protected:
	bool overHeated = false;
	float heatLevel = 0;
	float heatLosePerTick = 0.05;
	float heatGenerated = 0;

public:
	virtual void Fire(class AMech_RPGCharacter* target) override;
	virtual void Tick(float DeltaTime) override;
	virtual bool CanFire() override;
	virtual	void SetOwner(AMech_RPGCharacter* inOwner) override;
	
	UFUNCTION(BlueprintCallable, Category = "OverHeatWeapon")
	void Cooldown();

	UFUNCTION(BlueprintCallable, Category = "OverHeatWeapon")
	float GetHeatLevel();

	static AOverHeatWeapon* CreateOverHeatWeapon(UWorld* world, AMech_RPGCharacter* inOwner, FOverheatWeaponParams inSettings);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
		virtual float GetProgressBarPercent() override;
};
