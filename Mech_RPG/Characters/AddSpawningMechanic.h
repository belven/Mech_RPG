// Copyright of Explosive Industries

#pragma once

#include "BossMechanic.h"
#include "Delayed Events/SpawnTrigger.h"
#include "AddSpawningMechanic.generated.h"

/**
 *
 */
UCLASS(Blueprintable)
class MECH_RPG_API AAddSpawningMechanic : public ABossMechanic {
	GENERATED_BODY()
public:
	AAddSpawningMechanic();
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
		int32 spawnAmount = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	TSubclassOf<class AMech_RPGCharacter> classToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
		USpawnTrigger* trigger;

	UFUNCTION(BlueprintCallable, Category = "Spawn")
		void TriggerSpawn();

	UFUNCTION(BlueprintCallable, Category = "Spawn")
		void SetUpCharacter(AMech_RPGCharacter* character);

	UFUNCTION(BlueprintCallable, Category = "Spawn")
	void SetBoss(ABoss* newVal) override;
};
