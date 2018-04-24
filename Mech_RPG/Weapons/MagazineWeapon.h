// Copyright of Explosive Industries

#pragma once

#include "Weapon.h"
#include "MagazineWeapon.generated.h"

USTRUCT(BlueprintType)
struct FMagazineWeaponParams : public FWeaponParams
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		float reloadAmount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		float magazineSize;
};

UCLASS()
class MECH_RPG_API UMagazineWeapon : public UWeapon
{
	GENERATED_BODY()

protected:
	bool reloading = false;
	float reloadAmount = 1;
	float ammo = 10;
	float magazineSize = 10;
	float lastTime = 0;

public:
	virtual void UseWeapon(class AMech_RPGCharacter* target) override;
	virtual void Tick(float DeltaTime) override;

	virtual bool CanFire() override;
	virtual	void SetItemOwner(AMech_RPGCharacter* inOwner) override;

	UFUNCTION(BlueprintCallable, Category = "MagazineWeapon")
		void Reload();

	UFUNCTION(BlueprintCallable, Category = "MagazineWeapon")
		void FullReload();

	UFUNCTION(BlueprintCallable, Category = "MagazineWeapon")
		float GetMagazineSize();

	UFUNCTION(BlueprintCallable, Category = "MagazineWeapon")
		float GetAmmo();

	static UMagazineWeapon* CreateMagazineWeapon(AMech_RPGCharacter* inOwner, FMagazineWeaponParams inSettings);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
		virtual float GetProgressBarPercent() override;

	UFUNCTION(BlueprintCallable, Category = "Weapon")
		virtual FLinearColor GetProgressBarColour() override;
};
