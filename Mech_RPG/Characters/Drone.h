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
private:
	AMech_RPGCharacter * droneOwner;
public:
	ADrone();
	
	UFUNCTION(BlueprintCallable, Category = "Role")
		virtual void CreatePresetRole(ERole inRole = ERole::DPS, int32 grade = 0, int32 quaility = 0) override;

	UFUNCTION(BlueprintCallable, Category = "Health")
		virtual float GetMaxHealth() override;
	
	float GetHealthChangeModifier() override;

	UFUNCTION(BlueprintCallable, Category = "Drone")
		AMech_RPGCharacter * GetDroneOwner() { return droneOwner; }

	UFUNCTION(BlueprintCallable, Category = "Drone")
		void SetDroneOwner(AMech_RPGCharacter * val) { droneOwner = val; }


	 float GetDefenceModifier() override;
};
