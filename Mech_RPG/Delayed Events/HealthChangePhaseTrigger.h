// Copyright of Explosive Industries

#pragma once

#include "PhaseTrigger.h"
#include "HealthChangePhaseTrigger.generated.h"

class ABoss;
UCLASS(Blueprintable)
class MECH_RPG_API UHealthChangePhaseTrigger : public UPhaseTrigger
{
	GENERATED_BODY()

private:
	float healthPercent;
public:
	UFUNCTION(BlueprintCallable, Category = "PhaseTrigger")
		static UHealthChangePhaseTrigger* CreateHealthChangePhaseTrigger(ABoss* inBoss, float inHealthPercent, int32 inPhase);

	UFUNCTION(BlueprintCallable, Category = "PhaseTrigger")
		virtual void Trigger() override;

	UFUNCTION(BlueprintCallable, Category = "PhaseTrigger")
		void HealthChanged(FHealthChange healthChange);

};
