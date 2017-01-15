// Copyright of Explosive Industries

#pragma once

#include "Object.h"
#include "Array.h"
#include "EffectTimer.generated.h"

/**
 *
 */
UCLASS()
class MECH_RPG_API UEffectTimer : public UObject
{
	GENERATED_BODY()
private:
	UEffectTimer();
	~UEffectTimer();
	
	UPROPERTY()
		TArray<TEnumAsByte<EffectEnums::CrowdControl>> effects;

	UPROPERTY()
		bool remove = false;

	UPROPERTY()
		bool isComplete = false;

	UPROPERTY()
		float duration = 1;

	UPROPERTY()
		class AMech_RPGCharacter* target;

	UPROPERTY()
		FTimerHandle TimerHandle_EffectEnds;
public:
	UFUNCTION()
		static UEffectTimer* CreateEffectTimer(AMech_RPGCharacter* inTarget, float inDuration, TArray<TEnumAsByte<EffectEnums::CrowdControl>> inEffects, bool inRemove = false);

	UFUNCTION()
		void Activate();

	UFUNCTION()
		void Complete();
};
