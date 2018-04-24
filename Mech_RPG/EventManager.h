// Copyright of Explosive Industries

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Weapons/Weapon.h"
#include "EventManager.generated.h"

class AMech_RPGCharacter;
class UWeapon;

USTRUCT(BlueprintType)
struct FHealthChange
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		AMech_RPGCharacter* target = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		AMech_RPGCharacter* manipulator = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		UWeapon* weaponUsed = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		float changeAmount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		EDamageType damageType = EDamageType::Physical;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		bool heals = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		bool crit = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		bool ignoresArmour = false;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPreHealthChangeEvent, FHealthChange&, healthChange);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPostHealthChangeEvent, FHealthChange&, healthChange);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEntityKilled, FHealthChange&, healthChange);

UCLASS()
class MECH_RPG_API UEventManager : public UObject
{
	GENERATED_BODY()

private:
public:
	UPROPERTY()
		FPreHealthChangeEvent OnPreHealthChange;

	UPROPERTY()
		FPostHealthChangeEvent OnPostHealthChange;

	UPROPERTY()
		FEntityKilled OnEntityKilled;

	UFUNCTION(BlueprintCallable, Category = "Events")
		static void EntityKilled(FHealthChange entity);

	UFUNCTION(BlueprintCallable, Category = "Events")
		static void PreHealthChange(FHealthChange healthChange);

	UFUNCTION(BlueprintCallable, Category = "Events")
		static void PostHealthChange(FHealthChange healthChange);
};