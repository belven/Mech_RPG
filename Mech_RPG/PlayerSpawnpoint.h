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
	public:
		virtual void BeginPlay() override;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Role")
		TArray<TEnumAsByte<GroupEnums::Role>> allyRoles;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
			TSubclassOf<class AMech_RPGCharacter> playerClass;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Role")
			TEnumAsByte<GroupEnums::Role> playerRole;
	
	
};
