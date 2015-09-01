// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Mech_RPGCharacter.h"
#include "Boss.generated.h"

/**
 * 
 */
UCLASS()
class MECH_RPG_API ABoss : public AMech_RPGCharacter
{
	GENERATED_BODY()
private:
	bool canAttack;
	bool canMove;	

public:
	bool CanAttack();
	bool CanMove();
	void SetCanAttack(bool newVal);
	void SetCanMove(bool newVal);
};
