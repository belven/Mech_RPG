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
	TArray<EffectEnums::CrowdControl> effects;
	bool remove = false;
	float duration = 1;
	class AMech_RPGCharacter* target;
public:
	static UEffectTimer* CreateEffectTimer(AMech_RPGCharacter* inTarget, float inDuration, TArray<EffectEnums::CrowdControl> inEffects, bool inRemove = false);


};
