// Copyright of Explosive Industries

#pragma once

#include "SpawnTrigger.h"
#include "DamageSpawnTrigger.generated.h"

/**
 *
 */
UCLASS(Blueprintable)
class MECH_RPG_API UDamageSpawnTrigger : public USpawnTrigger {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
		float percent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
		float percentDamageTaken;

	UFUNCTION(BlueprintCallable, Category = "Spawn")
		static UDamageSpawnTrigger* CreateDamageSpawnTrigger(ABoss* inBoss, int32 inAmount, float inPercent);

	UFUNCTION(BlueprintCallable, Category = "Spawn")
		void BossHealthChange(FHealthChange& damage);

	UFUNCTION(BlueprintCallable, Category = "Spawn")
		void SetBoss(ABoss* inBoss) override;
};
