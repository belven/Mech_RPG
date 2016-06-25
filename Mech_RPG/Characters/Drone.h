// Copyright of Explosive Industries

#pragma once

#include "Characters/Mech_RPGCharacter.h"
#include "Drone.generated.h"

/**
 * 
 */
UCLASS()
class MECH_RPG_API ADrone : public AMech_RPGCharacter
{
	GENERATED_BODY()
public:
	ADrone();

	virtual void OutOfCombat() override;
	virtual void SetDead(bool newVal) override;

	UFUNCTION(BlueprintCallable, Category = "Role")
		virtual void CreatePresetRole(TEnumAsByte<GroupEnums::Role> inRole = GroupEnums::DPS, int32 grade = 0, int32 quaility = 0) override;
	
};
