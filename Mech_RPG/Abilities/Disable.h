#pragma once

#include "Abilities/Ability.h"
#include "Characters/Mech_RPGCharacter.h"
#include "Disable.generated.h"


UCLASS()
class MECH_RPG_API UDisable : public UAbility {
	GENERATED_BODY()
private:
	UPROPERTY()
	float duration;

	UPROPERTY()
	class AMech_RPGCharacter* targetCharacter;

public:
	UFUNCTION(BlueprintCallable, Category = "Ability")
		bool Activate(class AMech_RPGCharacter* target, FVector targetLocation = FVector::ZeroVector) override;

	UFUNCTION(BlueprintCallable, Category = "Ability")
		static UDisable* CreateDisable(float cooldown, AMech_RPGCharacter* inOwner, float inDuration);

	UFUNCTION(BlueprintCallable, Category = "Ability")
		virtual FString GetTooltipText() override;

	void DisableEnd();

	FTimerHandle TimerHandle_DisableEnd;

};
