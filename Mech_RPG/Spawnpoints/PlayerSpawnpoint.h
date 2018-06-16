// Copyright of Explosive Industries

#pragma once

#include "Spawnpoint.h"
#include "PlayerSpawnpoint.generated.h"

/**
 *
 */
UCLASS()
class MECH_RPG_API APlayerSpawnpoint : public ASpawnpoint
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere, Category = "Role")
	bool defaultSpawnpoint;

public:
	APlayerSpawnpoint();

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Role")
		TArray<ERole> allyRoles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
		TSubclassOf<class AMech_RPGCharacter> playerClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Role")
		ERole playerRole;
};
