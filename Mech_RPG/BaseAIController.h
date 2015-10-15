// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "BaseAIController.generated.h"

class AMech_RPGCharacter;

UCLASS()
class MECH_RPG_API ABaseAIController : public AAIController {
	GENERATED_BODY()

private:
	AMech_RPGCharacter* characterOwner;

	AMech_RPGCharacter* target;

	FCollisionQueryParams collision;
	FCollisionObjectQueryParams objectCollision;

	bool collisionSet = false;

	FHitResult hit;

	virtual void Tick(float DeltaTime) override;
public:
	ABaseAIController(const FObjectInitializer& ObjectInitializer);
	void AttackTarget(float DeltaTime);
	void FindTarget();

	void PerformAbility();
	void FireWeapon(AActor* hit);
	void SetupCollision();
	
	AMech_RPGCharacter* GetOwner();
	AMech_RPGCharacter* GetTarget();

	bool IsTargetValid(AMech_RPGCharacter* inTarget);

	void SetOwner(AMech_RPGCharacter* newVal);
	void SetTarget(AMech_RPGCharacter* newVal);

	void MoveToActor(AActor* target);
	void MoveToLocation(FVector location);

	UFUNCTION(BlueprintCallable, Category = "Group")
	void GroupMemberDamaged(AMech_RPGCharacter* attacker, AMech_RPGCharacter* damagedMember);

	TArray<AMech_RPGCharacter*> GetCharactersInRange(float range);

};
