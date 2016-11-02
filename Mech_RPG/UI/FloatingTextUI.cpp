// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "Characters/Mech_RPGCharacter.h"
#include "FloatingTextUI.h"
#include "WidgetAnimation.h"

UFloatingTextUI * UFloatingTextUI::CreateFloatingText(TSubclassOf<class UFloatingTextUI> floatingTextClass, FHealthChange inHealthChange)
{
	UFloatingTextUI* widget = CreateWidget<UFloatingTextUI>(inHealthChange.manipulator->GetWorld(), floatingTextClass);
	widget->healthChange = inHealthChange;
	widget->AddToViewport();
	widget->PlayAnimation(widget->textAnimation);
	return widget;
}

float UFloatingTextUI::GetText()
{
	return healthChange.healthChange;
}

FSlateColor UFloatingTextUI::GetTextColour()
{
	FLinearColor colour = FLinearColor::Red;
	
	if (healthChange.heals) {
		colour = FLinearColor::Green;

		if (healthChange.crit) {
			colour = FLinearColor::Yellow;
		}
	}
	else if (healthChange.crit) {
		colour = FLinearColor::Black;
	}

	return FSlateColor(colour);
}
