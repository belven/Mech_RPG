// Copyright of Explosive Industries

#pragma once

#include "Object.h"
#include "Delayed Events/AOEHealthChange.h"
#include "MiscLibrary.generated.h"

#define MIN(a,b) (a < b) ? (a) : (b)
#define MAX(a,b) (a > b) ? (a) : (b)

#define mIsChildOf(inUObject, checkClass) UMiscLibrary::IsChildOf(inUObject, checkClass)
#define CLAMP(value, max, min) (value = (MAX(MIN(value, max), min)))

UENUM(BlueprintType)
namespace GameEnums {
	enum Difficulty {
		Easy,
		Medium,
		Hard
	};
}

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

UCLASS()
class MECH_RPG_API UMiscLibrary : public UObject {
	GENERATED_BODY()

private:
	static UGroup* playerGroup;
	static TEnumAsByte<GameEnums::Difficulty> difficulty;
	static class AMech_RPGPlayerController* playerController;
	static FRotator cameraRot;

public:
	UFUNCTION(BlueprintCallable, Category = "Character")
		static float GetMissingHealth(AMech_RPGCharacter* character);

	static class AWeapon* CreatePresetWeapon(class UWorld* world, class AMech_RPGCharacter* inOwner, TEnumAsByte<WeaponEnums::WeaponType> weaponType, int32 weaponGrade, int32 weaponQuality);

	static TEnumAsByte<GameEnums::Difficulty> GetDifficulty();

	static const FString lnBreak;

	static const float MAX_HEALTH_CHANGE;

	template<class T> static void InitialiseArray(TArray<T>& emptyArray, T* items);

	static bool IsCrit(float critChance);

	static bool IsSuccess(float chance);

	static	float GetRandomPercent();

	static FColor GetRelativeColour(AMech_RPGCharacter * character);

	static void SetDifficulty(GameEnums::Difficulty newDifficulty);

	static bool CanSee(UWorld * world, FVector pointA, FVector pointB);

	UFUNCTION(BlueprintCallable, Category = "Character")
		static UGroup* GetPlayerGroup();

	UFUNCTION(BlueprintCallable, Category = "Character")
		static void SetPlayerGroup(UGroup* group);

	UFUNCTION(BlueprintCallable, Category = "Character")
		static AMech_RPGCharacter* GetPlayer();

	UFUNCTION(BlueprintCallable, Category = "Character")
		static float GetMissingHealthPercent(AMech_RPGCharacter* character);

	UFUNCTION(BlueprintCallable, Category = "Character")
		static float GetHealthPercent(AMech_RPGCharacter* character);

	UFUNCTION(BlueprintCallable, Category = "Character")
		static bool IsCharacterAlive(AMech_RPGCharacter* character);

	UFUNCTION(BlueprintCallable, Category = "Ability")
		static float GetAbilityCooldownPercent(UAbility* ability);

	UFUNCTION(BlueprintCallable, Category = "Ability")
		static float GetAbilityChannelPercent(UChannelledAbility* ability);

	UFUNCTION(BlueprintCallable, Category = "Character")
		static TArray<AMech_RPGCharacter*> GetCharactersInRange(float range, AActor* origin);

	static TArray<AMech_RPGCharacter*> GetCharactersInRange(float range, FVector location);

	//static TArray<AMech_RPGCharacter*> GetCharactersInRange(float range, AMech_RPGCharacter* origin);

	UFUNCTION(BlueprintCallable, Category = "Character")
		static bool IsMechCharacter(AActor* character);

	UFUNCTION(BlueprintCallable, Category = "Character")
		static bool IsChildOf(UObject* object, UClass* inClass);

	template<class T> static T GetRandomEnum(T end);

	template<class T> static T* SpawnCharacter(UWorld* world, FVector location, FRotator rotation, TSubclassOf<AMech_RPGCharacter> classToSpawn);

	UFUNCTION(BlueprintCallable, Category = "Character")
		static void OpenCharacterPane(UWorld* world);

	static	bool IsTargetValid(AMech_RPGCharacter* character, AMech_RPGCharacter* inTarget, EAffectedTeam affectedTeam);

	UFUNCTION(BlueprintCallable, Category = "Character")
		static float GetWidgetYaw(UCameraComponent* camera);

	UFUNCTION(BlueprintCallable, Category = "Character")
		static UWorld* GetActorWorld(AActor* actor);

	UFUNCTION(BlueprintCallable, Category = "Character")
		static class AMech_RPGPlayerController* GetPlayerController();

	UFUNCTION(BlueprintCallable, Category = "Character")
		static void SetPlayerController(AMech_RPGPlayerController* newController);

	UFUNCTION(BlueprintCallable, Category = "Character")
		static FRotator GetCameraRot() { return cameraRot; }

	UFUNCTION(BlueprintCallable, Category = "Character")
		static void SetCameraRot(FRotator val) { cameraRot = val; }

	UFUNCTION(BlueprintCallable, Category = "Character")
		static float GetMeleeRange(AMech_RPGCharacter* character);
	static FVector FindNavLocation(AActor* actor, float radius);
};
