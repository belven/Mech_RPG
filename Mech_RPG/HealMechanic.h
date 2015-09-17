// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BossMechanic.h"
#include "HealMechanic.generated.h"

/**
 * 
 */
UCLASS()
class MECH_RPG_API AHealMechanic : public ABossMechanic
{
	GENERATED_BODY()
	
public:
	AHealMechanic();
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Heal")
		float healAmount;
	
};
