// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "CharacterStats.h"
#include "Characters/Mech_RPGCharacter.h"
#include "Mech_RPGPlayerController.h"

#define mWBL UWidgetBlueprintLibrary

float UCharacterStats::GetHealthBarPercent()
{
	return owner != nullptr ? UMiscLibrary::GetHealthPercent(owner) : 0.0f;
}

FText UCharacterStats::GetRoleText()
{
	return owner != nullptr ? FindObject<UEnum>(ANY_PACKAGE, TEXT("GroupEnums"), true)->GetDisplayNameText(owner->GetRole()) : FText();
}

FString UCharacterStats::GetHealthText()
{
	FString healthtext("");

	if (owner != nullptr) {
		float health = round(owner->GetHealth());
		float maxHealth = round(owner->GetMaxHealth());
		FString healthString = FString::FromInt(health);
		FString maxHealthString = FString::FromInt(maxHealth);

		float healthLen = MIN(maxHealthString.Len(), 4);

		if (healthString.Len() < healthLen) {
			for (int i = 0; i < healthLen - healthString.Len(); i++) {
				healthtext += "0";
			}
		}

		healthtext += healthString;
		healthtext += "/";
		healthtext += maxHealthString;
	}

	return healthtext;
}

FString UCharacterStats::GetArmourText(DamageEnums::DamageType damageType)
{
	FString ArmourText("");

	if (owner != nullptr) {
		float armour = round(owner->GetTotalResistance(damageType)* owner->GetDefenceModifier());
		ArmourText += FString::FromInt(armour);
	}

	return ArmourText;
}

FString UCharacterStats::GetDPSText()
{
	FString DPSText("");

	if (owner != nullptr && owner->GetCurrentWeapon() != nullptr) {
		float dps = round(owner->GetCurrentWeapon()->GetChangeAmount() * owner->GetHealthChangeModifier());
		DPSText += FString::FromInt(dps);
	}

	return DPSText;
}

FReply UCharacterStats::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	startDrag = true;
	//return mWBL::CaptureMouse(mWBL::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton), this).NativeReply;

	return FReply::Handled();
}

FReply UCharacterStats::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	startDrag = false;
	//return mWBL::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton)->NativeReply;

	return FReply::Handled();
}

FReply UCharacterStats::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{

	return FReply::Handled();
}

FString UCharacterStats::GetMovementSpeedText()
{
	FString MovementText("");

	if (owner != nullptr) {
		float movement = round(owner->GetCharacterMovement()->MaxWalkSpeed * owner->GetMovementModifier());
		MovementText += FString::FromInt(movement);
	}

	return MovementText;
}

float UCharacterStats::GetCastBarPercent()
{
	if (UMiscLibrary::IsCharacterAlive(owner) && owner->Channelling()) {
		if (owner->GetCurrentAbility() != nullptr && owner->GetCurrentAbility()->GetClass()->IsChildOf(UChannelledAbility::StaticClass())) {
			return UMiscLibrary::GetAbilityChannelPercent(Cast<UChannelledAbility>(owner->GetCurrentAbility()));
		}
	}

	return 0.0f;
}

FLinearColor UCharacterStats::GetColour()
{
	if (owner != nullptr 
		&& owner->GetController() != nullptr 
		&& owner->GetController()->GetClass()->IsChildOf(AMech_RPGPlayerController::StaticClass())) {
		return FLinearColor(0.01, 0.3, 0); //Green
	}
	else {
		return FLinearColor(0.3, 0, 0.01); // Red
	}
}

float UCharacterStats::GetAbilityPercent()
{
	if (UMiscLibrary::IsCharacterAlive(owner)) {
		if (owner->GetCurrentAbility() != nullptr) {
			return UMiscLibrary::GetAbilityCooldownPercent(owner->GetCurrentAbility());
		}
	}

	return 0.0f;
}

ESlateVisibility UCharacterStats::GetChannelBarVis()
{
	if (owner != nullptr && owner->Channelling()) {
		return ESlateVisibility::Visible;
	}

	return ESlateVisibility::Collapsed;
}

float UCharacterStats::GetAbilityTimeRemaining()
{
	if (UMiscLibrary::IsCharacterAlive(owner)) {
		if (owner->GetCurrentAbility() != nullptr) {
			return owner->GetCurrentAbility()->GetCurrentTimeRemaining();
		}
	}

	return 0.0f;
}

AMech_RPGCharacter* UCharacterStats::GetOwner()
{
	return owner;
}

void UCharacterStats::SetOwner(AMech_RPGCharacter* newOwner)
{
	owner = newOwner;

	if(newOwner != nullptr) {
		newOwner->SetCharacterStats(this);
	}
}