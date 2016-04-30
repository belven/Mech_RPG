// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "BaseAIController.generated.h"

class AMech_RPGCharacter;

UCLASS()
class MECH_RPG_API ABaseAIController : public AAIController {
	GENERATED_BODY()

protected:
	AMech_RPGCharacter* characterOwner;
	AMech_RPGCharacter* target;
	FHitResult hit;
public:
	ABaseAIController();
	void AttackTarget(float DeltaTime);

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Group")
	virtual void OwnerPostBeginPlay(AMech_RPGCharacter* mech);
	void FindTarget(bool ally = false);


	bool PerformAbility(UAbility* ability);
	void FireWeapon(AActor* hit);
	bool ShouldHeal(UAbility* ability);

	UAbility* GetOwnerAbilityByTag(FString tag);
	
	AMech_RPGCharacter* GetOwner();
	AMech_RPGCharacter* GetTarget();

	bool IsTargetValid(AMech_RPGCharacter* inTarget, bool ally = false);

	void SetOwner(AMech_RPGCharacter* newVal);
	void SetTarget(AMech_RPGCharacter* newVal);

	void MoveToActor(AActor* target);
	void MoveToLocation(FVector location);

	UFUNCTION(BlueprintCallable, Category = "Group")
	void GroupMemberDamaged(AMech_RPGCharacter* attacker, AMech_RPGCharacter* damagedMember);

	TArray<AMech_RPGCharacter*> GetCharactersInRange(float range);

};
