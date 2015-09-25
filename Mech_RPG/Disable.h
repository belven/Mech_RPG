#pragma once

#include "Ability.h"
#include "Mech_RPGCharacter.h"
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
		void Activate(AMech_RPGCharacter* target, FVector targetLocation) override;

	UFUNCTION(BlueprintCallable, Category = "Ability")
		static UDisable* CreateDisable(float cooldown, AMech_RPGCharacter* inOwner, float inDuration);

	void DisableEnd();

	FTimerHandle TimerHandle_DisableEnd;

};
