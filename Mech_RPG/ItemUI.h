// Copyright of Explosive Industries

#pragma once

#include "Blueprint/UserWidget.h"
#include "ItemUI.generated.h"

class AItem;

UCLASS()
class MECH_RPG_API UItemUI : public UUserWidget
{
	GENERATED_BODY()
protected:
	AItem* owner;

public:
	UFUNCTION(BlueprintCallable, Category = "Item")
		void SetItem(AItem* inItem);

	UFUNCTION(BlueprintCallable, Category = "Item")
		AItem* GetItem();	
};
