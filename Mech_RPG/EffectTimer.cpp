// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "Mech_RPGCharacter.h"
#include "EffectTimer.h"

UEffectTimer* UEffectTimer::CreateEffectTimer(AMech_RPGCharacter* inTarget, float inDuration, TArray<EffectEnums::CrowdControl> inEffects, bool inRemove)
{
	UEffectTimer* timer = NewObject<UEffectTimer>(StaticClass());
	timer->effects = inEffects;
	timer->target = inTarget;
	timer->duration = inDuration;
	timer->remove = inRemove;
	return timer;
}
