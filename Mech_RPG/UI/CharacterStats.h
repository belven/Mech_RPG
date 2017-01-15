// Copyright of Explosive Industries

#pragma once

#include "Blueprint/UserWidget.h"
#include "Runtime/SlateCore/Public/Input/Reply.h"
#include "Runtime/UMG/Public/Blueprint/WidgetBlueprintLibrary.h"
#include "CharacterStats.generated.h"

UCLASS()
class MECH_RPG_API UCharacterStats : public UUserWidget
{
	GENERATED_BODY()
private:
	UPROPERTY()
	class AMech_RPGCharacter* owner;
	bool startDrag = false;
public:

	UFUNCTION(BlueprintCallable, Category = "Stats")
		FText GetRoleText();

	UFUNCTION(BlueprintCallable, Category = "Stats")
		FString GetHealthText();

	UFUNCTION(BlueprintCallable, Category = "Stats")
		FString GetArmourText(EDamageType damageType);

	UFUNCTION(BlueprintCallable, Category = "Stats")
		FString GetDPSText();

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	UFUNCTION(BlueprintCallable, Category = "Stats")
		FString GetMovementSpeedText();

	UFUNCTION(BlueprintCallable, Category = "Stats")
		float GetHealthBarPercent();

	UFUNCTION(BlueprintCallable, Category = "Stats")
		float GetCastBarPercent();

	UFUNCTION(BlueprintCallable, Category = "Stats")
		FLinearColor GetColour();

	UFUNCTION(BlueprintCallable, Category = "Stats")
		float GetAbilityPercent();

	UFUNCTION(BlueprintCallable, Category = "Stats")
		float GetAbilityTimeRemaining();

	UFUNCTION(BlueprintCallable, Category = "Stats")
		ESlateVisibility GetChannelBarVis();

	UFUNCTION(BlueprintCallable, Category = "Stats")
		AMech_RPGCharacter* GetOwner();

	UFUNCTION(BlueprintCallable, Category = "Stats")
		void SetOwner(class AMech_RPGCharacter* newOwner); 
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Abilities")
		void UpdateAbilityBar();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Abilities")
		void UpdateHealthBar();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Abilities")
		void UpdateWeaponBar();
};
