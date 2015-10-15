// Copyright of Explosive Industries

#pragma once

#include "Engine/TargetPoint.h"
#include "Mech_RPGCharacter.h"
#include "Spawnpoint.generated.h"

/**
 * 
 */
UCLASS()
class MECH_RPG_API ASpawnpoint : public ATargetPoint
{
	GENERATED_BODY()
public:
	ASpawnpoint();
	virtual void BeginPlay() override;
	int spawnAmount = 3;

	TSubclassOf<class AMech_RPGCharacter> classToSpawn;
	
	
	
};
