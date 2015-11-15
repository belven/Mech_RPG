// Copyright of Explosive Industries

#pragma once

#include "Ability.h"
#include "TargetedHealthChange.generated.h"

/**
 *
 */
UCLASS()
class MECH_RPG_API UTargetedHealthChange : public UAbility {
	GENERATED_BODY()
protected:
	float changeAmount;

public:

	UFUNCTION(BlueprintCallable, Category = "Ability")
		void Activate(AMech_RPGCharacter* target, FVector targetLocation) override;

	UFUNCTION(BlueprintCallable, Category = "Ability")
		static UTargetedHealthChange* CreateAbility(float cooldown, AMech_RPGCharacter* owner, float inChangeAmount, AOEEnums::AffectedTeam team);


};
