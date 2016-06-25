// Copyright of Explosive Industries

#pragma once

#include "Object.h"
#include "Characters/Boss.h"
#include "SpawnTrigger.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTriggerSpawn);

UCLASS(Blueprintable)
class MECH_RPG_API USpawnTrigger : public UObject {
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
		ABoss* boss;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
		int32 amount;

	UFUNCTION(BlueprintCallable, Category = "Spawn")
		static USpawnTrigger* CreateSpawnTrigger(ABoss* inBoss);

	FTriggerSpawn OnTrigger;

	UFUNCTION(BlueprintCallable, Category = "Spawn")
		virtual void SetBoss(ABoss* inBoss);

	UFUNCTION(BlueprintCallable, Category = "Spawn")
		ABoss* GetBoss();

	UFUNCTION(BlueprintCallable, Category = "Spawn")
		int32 GetAmount();

	UFUNCTION(BlueprintCallable, Category = "Spawn")
		void SetAmount(int32 inAmount);
};
