// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Mech_RPGCharacter.h"
#include "Boss.h"
#include "BossMechanic.generated.h"

/**
 *
 */
UCLASS()
class MECH_RPG_API ABossMechanic : public AMech_RPGCharacter {
	GENERATED_BODY()
private:
	ABoss* boss;

public:
	ABossMechanic();
	virtual	void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Boss")
		ABoss* GetBoss();

	UFUNCTION(BlueprintCallable, Category = "Boss")
		virtual void SetBoss(ABoss* newVal);

	void CreatePresetRole(TEnumAsByte<GroupEnums::Role> inRole) override;

};
