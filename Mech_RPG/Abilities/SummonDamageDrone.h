// Copyright of Explosive Industries

#pragma once

#include "Ability.h"
#include "SummonDamageDrone.generated.h"

/**
 * 
 */
UCLASS()
class MECH_RPG_API USummonDamageDrone : public UAbility
{
	GENERATED_BODY()
private:
	class ADrone* droneSummoned;

public:

	UFUNCTION(BlueprintCallable, Category = "Ability")
		bool Activate(class AMech_RPGCharacter* target, FVector targetLocation = FVector::ZeroVector) override;

	UFUNCTION(BlueprintCallable, Category = "Ability")
		static USummonDamageDrone* CreateAbility(float cooldown, AMech_RPGCharacter* owner);

	UFUNCTION(BlueprintCallable, Category = "Ability")
		virtual FString GetTooltipText() override;
	
};
