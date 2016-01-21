// Fill out your copyright notice in the Description page of Project Settings.

UENUM(BlueprintType)
namespace AOEEnums {
	enum AffectedTeam {
		Ally,
		Enemy
	};
}

#pragma once
#include "Engine/TargetPoint.h"
#include "Armour.h"
#include "AOEHealthChange.generated.h"

class AMech_RPGCharacter;

USTRUCT(BlueprintType)
struct FTempAOESettings {
	GENERATED_USTRUCT_BODY()
public:
	AMech_RPGCharacter* owner = NULL;
	AMech_RPGCharacter* target = NULL;

	FVector location = FVector::ZeroVector;
	UWorld* world = NULL;

	float radius = 100;
	float healthChange = 100;
	float duration = 5;
	float rate = 1;

	TEnumAsByte<AOEEnums::AffectedTeam>  affectedTeam = AOEEnums::Enemy;
	TEnumAsByte<DamageEnums::DamageType> damageType = DamageEnums::Physical;

	bool usesTarget = false;
};

UCLASS()
class MECH_RPG_API AAOEHealthChange : public ATargetPoint
{
	GENERATED_BODY()
	
private:
	FTempAOESettings settings;
	int timesRan;
	FTimerHandle TimerHandle_AOERate;
public:
	AAOEHealthChange();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Particles)
		UParticleSystemComponent* partclSystem;

	static AAOEHealthChange* CreateAOEHealthChange(FTempAOESettings inSettings);
	void Activate();
};
