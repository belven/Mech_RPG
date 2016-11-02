// Copyright of Explosive Industries

#pragma once

#include "Object.h"
#include "PhaseTrigger.generated.h"

class ABoss;

UCLASS(Blueprintable)
class MECH_RPG_API UPhaseTrigger : public UObject
{
	GENERATED_BODY()
private:

	UPROPERTY()
		ABoss* boss;

	UPROPERTY()
		int32 phaseToTrigger;
public:
	UFUNCTION(BlueprintCallable, Category = "PhaseTrigger")
	virtual void Trigger() { check(0 && "You must override this"); };

	UFUNCTION(BlueprintCallable, Category = "PhaseTrigger")
		 void SetBoss(ABoss* newBoss) {
		boss = newBoss;
	}

	UFUNCTION(BlueprintCallable, Category = "PhaseTrigger")
		 void SetPhase(int32 newPhase) {
		phaseToTrigger = newPhase;
	}

	UFUNCTION(BlueprintCallable, Category = "PhaseTrigger")
		FORCEINLINE ABoss* GetBoss() {
		return boss;
	}

	UFUNCTION(BlueprintCallable,Category =  "PhaseTrigger")
		FORCEINLINE int32 GetPhase() {
		return phaseToTrigger;
	}
};
