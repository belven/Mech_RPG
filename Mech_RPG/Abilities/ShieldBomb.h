// Copyright of Explosive Industries

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Ability.h"
#include "ShieldBomb.generated.h"

/**
 * 
 */
UCLASS()
class MECH_RPG_API UShieldBomb : public UAbility
{
	GENERATED_BODY()
	
private:
	float shieldAmount;
	class AMech_RPGCharacter* shieldTarget;
	int count;
	float shieldHealth;
public:

	UFUNCTION(BlueprintCallable, Category = "Ability")
		bool Activate(class AMech_RPGCharacter* target, FVector targetLocation = FVector::ZeroVector) override;

	UFUNCTION(BlueprintCallable, Category = "Ability")
		static UShieldBomb* CreateShieldBomb(float cooldown, AMech_RPGCharacter* owner, float inShieldAmount);

	UFUNCTION(BlueprintCallable, Category = "Ability")
		virtual FString GetTooltipText() override;

	UFUNCTION(BlueprintCallable, Category = "Shield")
		void ChangeHealth(FHealthChange& healthChange);
	
	
};
