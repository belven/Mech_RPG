// Copyright of Explosive Industries

#pragma once

#include "Weapon.h"
#include "MagazineWeapon.generated.h"

USTRUCT(BlueprintType)
struct FMagazineWeaponParams : public FWeaponParams {
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		float reloadAmount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		float magazineSize;
};

UCLASS()
class MECH_RPG_API AMagazineWeapon : public AWeapon {
	GENERATED_BODY()

private:
	bool reloading = false;
	float reloadAmount = 1;
	float ammo = 10;
	float magazineSize = 10;
	float lastTime = 0;

public:
	virtual void Fire(class AMech_RPGCharacter* target, AMech_RPGCharacter* owner) override;
	virtual void Tick(float DeltaTime) override;
	virtual bool CanFire() override;

	UFUNCTION(BlueprintCallable, Category = "MagazineWeapon")
		float GetMagazineSize();

	UFUNCTION(BlueprintCallable, Category = "MagazineWeapon")
		float GetAmmo();

	static AMagazineWeapon* CreateMagazineWeapon(AActor* inOwner, FMagazineWeaponParams inSettings);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
		virtual float GetProgressBarPercent() override;
};
