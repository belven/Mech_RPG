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
	int32 phase = 0;
public:
	virtual void CreatePresetRole(TEnumAsByte<GroupEnums::Role> inRole = GroupEnums::DPS, int32 grade = 0, int32 quaility = 0) override;

	virtual void Tick(float DeltaTime) override;

	
	int32 Phase() const { return phase; }

	void SetPhase(int32 val);
};