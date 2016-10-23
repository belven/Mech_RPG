// Copyright of Explosive Industries

#pragma once

#include "Object.h"
#include "ModifierTimer.generated.h"


UENUM(BlueprintType)
namespace ModifierEnums {
	enum ModifierType {
		Speed,
		HealthChange,
		Defence
	};
}

UCLASS()
class MECH_RPG_API UModifierTimer : public UObject
{
	GENERATED_BODY()

	TMap<ModifierEnums::ModifierType, float> modifers;
	float duration = 1;
	class AMech_RPGCharacter* target;
	FTimerHandle TimerHandle_ModifierEnds;

	void ModifyTarget(TPair<ModifierEnums::ModifierType, float> modifier, bool remove = false);
public:
	static UModifierTimer* CreateEffectTimer(class AMech_RPGCharacter* inTarget, float inDuration, TMap<ModifierEnums::ModifierType, float> inModifers);
	void Activate();
	void Complete();
};
