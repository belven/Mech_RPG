// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BaseAIController.h"
#include "AllyAIController.generated.h"

/**
 *
 */
UCLASS()
class MECH_RPG_API AAllyAIController : public ABaseAIController
{
	GENERATED_BODY()
protected:
	FVector playerControlledLocation;
public:
	AAllyAIController();

	virtual	void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	bool ShouldMoveToPlayer(AMech_RPGCharacter* player);
	FVector GetPlayerControlledLocation();
	void SetPlayerControlledLocation(FVector newVal);
};
