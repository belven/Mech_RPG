// Copyright of Explosive Industries

#pragma once

#include "Blueprint/UserWidget.h"
#include "FloatingTextUI.generated.h"

/**
 *
 */
UCLASS()
class MECH_RPG_API UFloatingTextUI : public UUserWidget
{
	GENERATED_BODY()
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Floating Text")
	struct FHealthChange healthChange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Floating Text")
		float delay = 0.4; 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	class UWidgetAnimation* textAnimation;
	
public:
	static UFloatingTextUI* CreateFloatingText(TSubclassOf<class UFloatingTextUI> floatingTextClass, struct FHealthChange inHealthChange);

	UFUNCTION(BlueprintCallable, Category = "Floating Text")
		float GetText();

	UFUNCTION(BlueprintCallable, Category = "Floating Text")
		FSlateColor GetTextColour();
};
