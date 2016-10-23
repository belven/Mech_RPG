// Copyright of Explosive Industries

#pragma once

#include "Object.h"
#include "MiscLibrary.generated.h"


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

	static TEnumAsByte<GameEnums::Difficulty> GetDifficulty();

	static const FString lnBreak;

	static const float MAX_HEALTH_CHANGE;

	template<class T> static void InitialiseArray(TArray<T>& emptyArray, T* items);

	static bool IsCrit(float critChance);

	static FColor GetRelativeColour(AMech_RPGCharacter * character);

	static void SetDifficulty(GameEnums::Difficulty newDifficulty);

	static bool CanSee(UWorld * world, FVector pointA, FVector pointB);

	UFUNCTION(BlueprintCallable, Category = "Character")
		static UGroup* GetPlayerGroup();

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

	UFUNCTION(BlueprintCallable, Category = "Character")
		static bool IsCover(AActor* character);

	UFUNCTION(BlueprintCallable, Category = "Character")
		static bool IsMechCharacter(AActor* character);

	UFUNCTION(BlueprintCallable, Category = "Character")
		static bool IsChildOf(UObject* object, UClass* inClass);

	static int GetRandomEnum(int end);

	template<class T> static T* SpawnCharacter(UWorld* world, FVector location, FRotator rotation, TSubclassOf<AMech_RPGCharacter> classToSpawn);

	UFUNCTION(BlueprintCallable, Category = "Character")
		static void OpenCharacterPane(UWorld* world);

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
};
