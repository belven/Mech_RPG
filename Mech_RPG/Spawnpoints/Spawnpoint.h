// Copyright of Explosive Industries

#pragma once

#include "Engine/TargetPoint.h"
#include "Characters/Mech_RPGCharacter.h"
#include "Spawnpoint.generated.h"

/**
 *
 */
UCLASS()
class MECH_RPG_API ASpawnpoint : public ATargetPoint {
	GENERATED_BODY()
private:
	static TMap<UClass*, TArray<ASpawnpoint*>> spawnPoints;
public:
	ASpawnpoint();

	static float defaultSpawnRadius;

	AMech_RPGCharacter* SpawnCharacter(TSubclassOf<class AMech_RPGCharacter> spawnClass, int spawnRadius);
	void SetUpCharacter(AMech_RPGCharacter * character, UGroup * group, ERole role);

	static void AdjustCharacterLocationByCapsule(AMech_RPGCharacter* character);

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loadout")
		TEnumAsByte<TeamEnums::Team> team = TeamEnums::Mercenaries;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
		int32 spawnAmount = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
		int32 bonusGrade = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
		int32 bonusQuality = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
		TSubclassOf<class AMech_RPGCharacter> classToSpawn;

	static TArray<ASpawnpoint*>* GetSpawnpoints(UClass* classToFind);
};
