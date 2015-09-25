// Copyright of Explosive Industries

#pragma once

#include "BossMechanic.h"
#include "DamageBoostMechanic.generated.h"

UCLASS()
class MECH_RPG_API ADamageBoostMechanic : public ABossMechanic {
	GENERATED_BODY()
public:


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float boostAmount;

	virtual	void BeginPlay() override;

	virtual	void SetDead(bool newVal)override; 
};
