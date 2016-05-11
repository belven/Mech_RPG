// Copyright of Explosive Industries

#pragma once

#include "Blueprint/UserWidget.h"
#include "ItemUI.generated.h"

class AItem;
class UInventoryUI;

UCLASS()
class MECH_RPG_API UItemUI : public UUserWidget
{
	GENERATED_BODY()
protected:
	AItem* owner;
	UInventoryUI* inventory;

public:
	UFUNCTION(BlueprintCallable, Category = "Item")
		void SetItem(AItem* inItem);

	UFUNCTION(BlueprintCallable, Category = "Item")
		AItem* GetItem();

	UFUNCTION(BlueprintCallable, Category = "Item")
		UInventoryUI* GetInventory();

	UFUNCTION(BlueprintCallable, Category = "Item")
		void SetInventory(UInventoryUI* val);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Item")
		void SelectItem();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Item")
		void DeselectItem();
};
