// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "HealthChangePhaseTrigger.h"
#include "Characters/Boss.h"

UHealthChangePhaseTrigger* UHealthChangePhaseTrigger::CreateHealthChangePhaseTrigger(ABoss* inBoss, float inHealthPercent, int32 inPhase)
{
	UHealthChangePhaseTrigger* trigger = NewObject<UHealthChangePhaseTrigger>(UHealthChangePhaseTrigger::StaticClass());
	trigger->SetBoss(inBoss);
	trigger->SetPhase(inPhase);
	trigger->healthPercent = inHealthPercent;
	trigger->GetBoss()->OnPostHealthChange.AddDynamic(trigger, &UHealthChangePhaseTrigger::HealthChanged);
	return trigger;
}

void UHealthChangePhaseTrigger::Trigger()
{
	GetBoss()->SetPhase(GetPhase());
	GetBoss()->OnPostHealthChange.RemoveDynamic(this, &UHealthChangePhaseTrigger::HealthChanged);
}

void UHealthChangePhaseTrigger::HealthChanged(FHealthChange healthChange)
{
	int32 bossPhase = GetBoss()->GetPhase();
	int32 trigerPhase = GetPhase() - 1;

	float currentHealthPercent = UMiscLibrary::GetHealthPercent(GetBoss());

	if (currentHealthPercent <= healthPercent) {
		if (bossPhase == trigerPhase) {
			Trigger();
		}
	}
}
