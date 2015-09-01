// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Mech_RPGCharacter.h"
#include "Boss.h"
#include "BossMechanic.generated.h"

/**
 * 
 */
UCLASS()
class MECH_RPG_API ABossMechanic : public AMech_RPGCharacter
{
	GENERATED_BODY()
private:
	ABoss* boss;	
	
public:
	virtual	void BeginPlay() override;

	ABoss* GetBoss();
	void SetBoss(ABoss* newVal);
	
};
