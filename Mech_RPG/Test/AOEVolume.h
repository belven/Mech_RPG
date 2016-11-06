// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Volume.h"
#include "AOEVolume.generated.h"

class AMech_RPGCharacter;

USTRUCT(BlueprintType)
struct FAOESettings {
	GENERATED_USTRUCT_BODY()
public:
	FVector Location;
	UWorld* world;
	float radius;
	float inHealthChange;
	int inAffectedTeam;
	float duration;
	AMech_RPGCharacter* owner;
};

UCLASS()
class MECH_RPG_API AAOEVolume : public AVolume {
	GENERATED_BODY()

private:
	float healthChange;
	int affectedTeam;
	float maxDuration;
	float currentDuration;
	AMech_RPGCharacter* owner;

public:
	static AAOEVolume* CreateAAOEVolume(FAOESettings settings);
	virtual void Tick(float DeltaTime) override;


};
