// Copyright of Explosive Industries

#pragma once

#include "Weapon.h"
#include "OverHeatWeapon.generated.h"

/**
 *
 */
UCLASS()
class MECH_RPG_API AOverHeatWeapon : public AWeapon {
	GENERATED_BODY()

private:
	bool overHeated = false;
	float heatLevel = 0;
	float heatLosePerTick = 0.05;

public:
	virtual void Fire(class AMech_RPGCharacter* target, AMech_RPGCharacter* owner) override;
	virtual void Tick(float DeltaTime) override;
	virtual bool CanFire() override;

	UFUNCTION(BlueprintCallable, Category = "OverHeatWeapon")
	float GetHeatLevel();

	static AOverHeatWeapon* CreateOverHeatWeapon(AActor* inOwner, FWeaponParams inSettings);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
		virtual float GetProgressBarPercent() override;
};
