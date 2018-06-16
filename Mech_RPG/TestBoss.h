// Copyright of Explosive Industries

#pragma once

#include "Characters/Boss.h"
#include "TestBoss.generated.h"

/**
 * 
 */
UCLASS()
class MECH_RPG_API ATestBoss : public ABoss
{
	GENERATED_BODY()
	
private:
public:
	virtual void CreatePresetRole(ERole inRole = ERole::DPS, int32 grade = 0, int32 quaility = 0) override;

	void SetPhase(int32 val) override;
};