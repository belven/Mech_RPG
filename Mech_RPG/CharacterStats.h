// Copyright of Explosive Industries

#pragma once

#include "Blueprint/UserWidget.h"
#include "CharacterStats.generated.h"

/**
 *
 */
UCLASS()
class MECH_RPG_API UCharacterStats : public UUserWidget
{
	GENERATED_BODY()
private:
	UPROPERTY()
	class AMech_RPGCharacter* owner;
public:

	UFUNCTION(BlueprintCallable, Category = "Stats")
		FText GetRoleText();

	UFUNCTION(BlueprintCallable, Category = "Stats")
		FString GetHealthText();

	UFUNCTION(BlueprintCallable, Category = "Stats")
		FString GetArmourText(DamageEnums::DamageType damageType);

	UFUNCTION(BlueprintCallable, Category = "Stats")
		FString GetDPSText();

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
};
