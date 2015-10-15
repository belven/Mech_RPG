// Copyright of Explosive Industries

#pragma once

#include "Engine/TargetPoint.h"
#include "Mech_RPGCharacter.h"
#include "Spawnpoint.generated.h"

/**
 *
 */
UCLASS()
class MECH_RPG_API ASpawnpoint : public ATargetPoint {
	GENERATED_BODY()
public:
	ASpawnpoint();
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
		int32 spawnAmount = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
		TSubclassOf<class AMech_RPGCharacter> classToSpawn;
};
