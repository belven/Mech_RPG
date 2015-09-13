// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Ability.h"
#include "TestAOE.generated.h"

UCLASS()
class MECH_RPG_API UTestAOE : public UAbility {
	GENERATED_BODY()
		float damage;
public:
	UFUNCTION(BlueprintCallable, Category = "Ability")
		void Activate(AMech_RPGCharacter* target, FVector targetLocation) override;

	UFUNCTION(BlueprintCallable, Category = "Ability")
		static UTestAOE* CreateAbility(float cooldown, AMech_RPGCharacter* owner, float inDamage);

};
