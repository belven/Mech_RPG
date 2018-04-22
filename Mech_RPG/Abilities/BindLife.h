// Copyright of Explosive Industries

#pragma once

#include "CoreMinimal.h"
#include "Ability.h"
#include "EventManager.h"
#include "BindLife.generated.h"

/**
 * 
 */
UCLASS()
class MECH_RPG_API UBindLife : public UAbility
{
	GENERATED_BODY()
private:
	class AMech_RPGCharacter* boundTarget;
	float absorbPercent;
	float damageReduction;
public:

	UFUNCTION(BlueprintCallable, Category = "Ability")
		bool Activate(class AMech_RPGCharacter* target, FVector targetLocation = FVector::ZeroVector) override;

	UFUNCTION(BlueprintCallable, Category = "Ability")
		static UBindLife* CreateBindLife(float cooldown, AMech_RPGCharacter* owner, float inMaxAmount, float inDamageReduction);

	UFUNCTION(BlueprintCallable, Category = "Ability")
		virtual FString GetTooltipText() override;

	UFUNCTION(BlueprintCallable, Category = "Shield")
		void ChangeHealth(FHealthChange& healthChange);
	
	
	
	
};
