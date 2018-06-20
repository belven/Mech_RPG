// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Engine/TargetPoint.h"
#include "Enums.h"
#include "AOEHealthChange.generated.h"

class AMech_RPGCharacter;

USTRUCT(BlueprintType)
struct FTempAOESettings
{
	GENERATED_USTRUCT_BODY()
public:
	AMech_RPGCharacter* owner = nullptr;
	AMech_RPGCharacter* target = nullptr;

	FVector location = FVector::ZeroVector;
	UWorld* world = nullptr;

	float radius = 100;
	float healthChange = 100;
	float duration = 5;
	float rate = 1;

	EAffectedTeam affectedTeam = EAffectedTeam::Enemy;
	EDamageType damageType = EDamageType::Physical;

	bool usesTarget = false;
	bool heals = false;
	bool ignoresArmour = false;

	UParticleSystem* particleSystem = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		UAbility* abilityUsed = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		UWeapon* weaponUsed = nullptr;
};

UCLASS()
class MECH_RPG_API AAOEHealthChange : public ATargetPoint
{
	GENERATED_BODY()

private:
	FTempAOESettings settings;
	float currentLifetime;
	FTimerHandle TimerHandle_AOERate;
public:
	AAOEHealthChange();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Particles)
		UParticleSystemComponent* particleSystem;

	static AAOEHealthChange* CreateAOEHealthChange(FTempAOESettings inSettings);
	void Activate();

	void PerformHealthChange(AMech_RPGCharacter* character);

	static FVector GetLocationToUse(FTempAOESettings inSettings);
	bool CanAffect(AMech_RPGCharacter* character);
	float GetHealthChange(AMech_RPGCharacter* character);
	void CreateDebugSphere(FVector locationToUse);
};
