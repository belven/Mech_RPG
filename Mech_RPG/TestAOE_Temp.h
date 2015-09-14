// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "TestAOE_Temp.generated.h"

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

	int affectedTeam = 0;
	bool usesTarget = false;
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
