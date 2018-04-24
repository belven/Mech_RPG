// Copyright of Explosive Industries

#pragma once

#include "Blueprint/UserWidget.h"
#include "ItemUI.generated.h"

class UItem;
class UInventoryUI;

UCLASS()
class MECH_RPG_API UItemUI : public UUserWidget
{
	GENERATED_BODY()
protected:
	UItem* owner;
	UInventoryUI* inventory;

public:
	UFUNCTION(BlueprintCallable, Category = "Item")
		void SetItem(UItem* inItem);

	UFUNCTION(BlueprintCallable, Category = "Item")
		UItem* GetItem();

	UFUNCTION(BlueprintCallable, Category = "Item")
		UInventoryUI* GetInventory();

	UFUNCTION(BlueprintCallable, Category = "Item")
		void SetInventory(UInventoryUI* val);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Item")
		void SelectItem();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Item")
		void DeselectItem();
};
