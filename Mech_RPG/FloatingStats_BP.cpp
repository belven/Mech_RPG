// Copyright of Explosive Industries
#pragma once
#include "Mech_RPG.h"
#include "FloatingStats_BP.h"
#include "Mech_RPGCharacter.h"

float UFloatingStats_BP::GetHealthBarPercent()
{
	return owner != nullptr ? UMiscLibrary::GetHealthPercent(owner) : 0.0f;
}

float UFloatingStats_BP::GetCastBarPercent()
{
	if (UMiscLibrary::IsCharacterAlive(owner) && owner->Channelling()) {
		if (owner->GetCurrentAbility() != nullptr && owner->GetCurrentAbility()->GetClass()->IsChildOf(UChannelledAbility::StaticClass())) {
			return UMiscLibrary::GetAbilityChannelPercent(Cast<UChannelledAbility>(owner->GetCurrentAbility()));
		}
	}

	return 0.0f;
}

FLinearColor UFloatingStats_BP::GetHealthBarFillColorAndOpacity()
{
	if (owner != nullptr && owner->team == TeamEnums::Paladins) {
		return FLinearColor(0, 0.79, 0.067); //Green
	}
	else {
		return FLinearColor(1, 0, 0.064); // Red
	}
}

ESlateVisibility UFloatingStats_BP::GetChannelBarVis()
{
	if (owner != nullptr && owner->Channelling()) {
		return ESlateVisibility::Visible;
	}

	return ESlateVisibility::Collapsed;
}

void UFloatingStats_BP::SetOwner(AMech_RPGCharacter* newOwner)
{
	owner = newOwner;
}

void UFloatingStats_BP::Tick(float DeltaTime)
{
	AMech_RPGCharacter* player = UMiscLibrary::GetPlayer();

	if (player != nullptr) {
		FRotator rot(-player->GetTopDownCamera()->GetComponentRotation().Pitch, UMiscLibrary::GetWidgetYaw(player->GetTopDownCamera()) + 90, 0);

		owner->GetStats()->SetWorldRotation(rot);
	}
}
