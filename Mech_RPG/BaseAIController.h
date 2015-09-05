// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "BaseAIController.generated.h"

class AMech_RPGCharacter;

UCLASS()
class MECH_RPG_API ABaseAIController : public AAIController {
	GENERATED_BODY()

private:
	AMech_RPGCharacter* owner;
	AMech_RPGCharacter* target;
	FCollisionQueryParams collision;
	bool collisionSet = false;
	FHitResult hit;

	virtual void Tick(float DeltaTime) override;
public:
	void AttackTarget(float DeltaTime);
	void FindTarget();

	AMech_RPGCharacter* GetOwner();
	AMech_RPGCharacter* GetTarget();

	bool IsTargetValid();

	void SetOwner(AMech_RPGCharacter* newVal);
	void SetTarget(AMech_RPGCharacter* newVal);

	UFUNCTION()
		void OnOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapEnd(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
