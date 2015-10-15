// Copyright of Explosive Industries

#pragma once

#include "Spawnpoint.h"
#include "BossSpawnpoint.generated.h"

/**
 *
 */
UCLASS()
class MECH_RPG_API ABossSpawnpoint : public ASpawnpoint {
	GENERATED_BODY()


public:
	ABossSpawnpoint();
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
		TSubclassOf<class ABoss> bossClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
		TSubclassOf<class ABossMechanic> mechanicClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
		bool spawnAdds = true;
};
