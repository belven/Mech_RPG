// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "TestAOE_Temp.generated.h"

class AMech_RPGCharacter;

USTRUCT(BlueprintType)
struct FTempAOESettings {
	GENERATED_USTRUCT_BODY()
public:
	AMech_RPGCharacter* owner;
	AMech_RPGCharacter* target;

	FVector location;
	UWorld* world;

	float radius;
	float healthChange;
	float duration;
	float rate;

	int affectedTeam;
	bool usesTarget;
};

UCLASS()
class MECH_RPG_API UTestAOE_Temp : public UObject
{
	GENERATED_BODY()
	
private:
	FTempAOESettings settings;
	int timesRan;
	FTimerHandle TimerHandle_AOERate;
public:
	static UTestAOE_Temp* CreateTestAOE(FTempAOESettings inSettings);
	void Activate();
};
