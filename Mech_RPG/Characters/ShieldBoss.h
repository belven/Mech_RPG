// Copyright of Explosive Industries

#pragma once

#include "CoreMinimal.h"
#include "Characters/Boss.h"
#include "ShieldBoss.generated.h"

/**
 *
 */
UCLASS()
class MECH_RPG_API AShieldBoss : public ABoss
{
	GENERATED_BODY()


public:
	virtual void CreatePresetRole(TEnumAsByte<GroupEnums::Role> inRole = GroupEnums::DPS, int32 grade = 0, int32 quaility = 0) override;

	void SetPhase(int32 val) override;

};
