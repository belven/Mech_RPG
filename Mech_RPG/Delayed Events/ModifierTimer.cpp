// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "ModifierTimer.h"
#include "Characters/Mech_RPGCharacter.h"

void UModifierTimer::ModifyTarget(TPair<ModifierEnums::ModifierType, float> modifier, bool remove)
{
	float value = remove ? -modifier.Value : modifier.Value;

	switch (modifier.Key) {
	case ModifierEnums::Speed:
		target->SetSpeedModifier(target->GetSpeedModifier() + value);
		break;
	case ModifierEnums::HealthChange:
		target->SetHealthChangeModifier(target->GetHealthChangeModifier() + value);
		break;
	case ModifierEnums::Defence:
		target->SetDefenceModifier(target->GetDefenceModifier() + value);
		break;
	}
}

UModifierTimer* UModifierTimer::CreateEffectTimer(class AMech_RPGCharacter* inTarget, float inDuration, TMap<ModifierEnums::ModifierType, float> inModifers)
{
	UModifierTimer* timer = NewObject<UModifierTimer>(StaticClass());
	timer->modifers = inModifers;
	timer->target = inTarget;
	timer->duration = inDuration;
	timer->Activate();
	return timer;
}

void UModifierTimer::Activate() {
	if (UMiscLibrary::IsCharacterAlive(target)) {
		for (TPair<ModifierEnums::ModifierType, float> modifier : modifers) {
			ModifyTarget(modifier);
		}

		target->GetWorld()->GetTimerManager().SetTimer(TimerHandle_ModifierEnds, this, &UModifierTimer::Complete, duration);
	}
}

void UModifierTimer::Complete()
{
	target->GetWorld()->GetTimerManager().ClearTimer(TimerHandle_ModifierEnds);
	for (TPair<ModifierEnums::ModifierType, float> modifier : modifers) {
		ModifyTarget(modifier, true);
	}
}
