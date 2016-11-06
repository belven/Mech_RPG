// Copyright of Explosive Industries

#pragma once

#include "GameFramework/Volume.h"
#include "ScalingVolume.generated.h"

/**
 * 
 */
UCLASS()
class MECH_RPG_API AScalingVolume : public AVolume
{
	GENERATED_BODY()
public:

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
			TSubclassOf<class AActor> classToSpawn;

		virtual void BeginPlay() override;
	
	
};
