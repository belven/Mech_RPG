// Copyright of Explosive Industries

#pragma once

#include "Object.h"
#include "MiscLibrary.generated.h"


/**
 *
 */
UCLASS()
class MECH_RPG_API UMiscLibrary : public UObject {
	GENERATED_BODY()

private:
	static UGroup* playerGroup;

public:
	UFUNCTION(BlueprintCallable, Category = "Character")
		static float GetMissingHealth(AMech_RPGCharacter* character);
	
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

	UFUNCTION(BlueprintCallable, Category = "Character")
		static bool IsCover(AActor* character);

	UFUNCTION(BlueprintCallable, Category = "Character")
		static bool IsMechCharacter(AActor* character);

	static int GetRandomEnum(int end);

	template<class T> static T* SpawnCharacter(UWorld* world, FVector location, FRotator rotation, TSubclassOf<AMech_RPGCharacter> classToSpawn);

	UFUNCTION(BlueprintCallable, Category = "Character")
		static void OpenCharacterPane(UWorld* world);

	UFUNCTION(BlueprintCallable, Category = "Character")
		static float GetWidgetYaw(UCameraComponent* camera);

	UFUNCTION(BlueprintCallable, Category = "Character")
		static UWorld* GetActorWorld(AActor* actor);

};
