// Copyright of Explosive Industries

#pragma once

#include "Blueprint/UserWidget.h"
#include "MiscLibrary.h"
#include "Math/Color.h"
#include "FloatingStats_BP.generated.h"

/**
 *
 */
UCLASS()
class MECH_RPG_API UFloatingStats_BP : public UUserWidget
{
	GENERATED_BODY()
private:
	UPROPERTY()
	class AMech_RPGCharacter* owner;

public:
	UFUNCTION(BlueprintCallable, Category = "Stats")
		float GetHealthBarPercent();

	UFUNCTION(BlueprintCallable, Category = "Stats")
		float GetCastBarPercent();

	UFUNCTION(BlueprintCallable, Category = "Stats")
		FLinearColor GetHealthBarFillColorAndOpacity();

	UFUNCTION(BlueprintCallable, Category = "Stats")
		ESlateVisibility GetChannelBarVis();

	UFUNCTION(BlueprintCallable, Category = "Stats")
		FText GetRoleText();

	void SetOwner(class AMech_RPGCharacter* newOwner);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Stats")
		void UpdateHealthBar();

	void Tick(float DeltaTime);
};
