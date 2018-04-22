// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "Characters/Mech_RPGCharacter.h"
#include "EffectTimer.h"

UEffectTimer::UEffectTimer()
{
	SetFlags(EObjectFlags::RF_MarkAsRootSet);
}

UEffectTimer::~UEffectTimer()
{
	Complete();
}

UEffectTimer* UEffectTimer::CreateEffectTimer(AMech_RPGCharacter* inTarget, float inDuration, TArray<TEnumAsByte<EffectEnums::CrowdControl>> inEffects, bool inRemove)
{
	UEffectTimer* timer = NewObject<UEffectTimer>(StaticClass());
	timer->effects = inEffects;
	timer->target = inTarget;
	timer->duration = inDuration;
	timer->remove = inRemove;
	timer->Activate();
	return timer;
}

void UEffectTimer::Activate()
{
	if (UMiscLibrary::IsCharacterAlive(target))
	{
		for (EffectEnums::CrowdControl effect : effects)
		{
			target->ApplyCrowdControl(effect, remove);
		}

		target->GetWorld()->GetTimerManager().SetTimer(TimerHandle_EffectEnds, this, &UEffectTimer::Complete, duration);
	}
}

void UEffectTimer::Complete()
{
	if (!isComplete && UMiscLibrary::IsCharacterAlive(target))
	{
		isComplete = true;

		target->GetWorld()->GetTimerManager().ClearTimer(TimerHandle_EffectEnds);
		for (EffectEnums::CrowdControl effect : effects)
		{
			target->ApplyCrowdControl(effect, !remove);
		}

		ClearFlags(EObjectFlags::RF_MarkAsRootSet);
		SetFlags(EObjectFlags::RF_Transient);
	}
}
