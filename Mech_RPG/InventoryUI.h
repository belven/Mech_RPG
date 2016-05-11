// Copyright of Explosive Industries

#pragma once

#include "Blueprint/UserWidget.h"
#include "InventoryUI.generated.h"

class AMech_RPGCharacter;
class UItemUI;

UCLASS()
class MECH_RPG_API UInventoryUI : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY()
		AMech_RPGCharacter* owner;

	UPROPERTY()
		TArray<UItemUI*> items;

	UPROPERTY()
		TArray<UItemUI*> selectetItems;

public:
	UFUNCTION(BlueprintCallable, Category = "Inventory")
		void SetOwner(AMech_RPGCharacter* inOwner);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
		AMech_RPGCharacter* GetOwner();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Inventory")
		void GenerateInventory();

	UFUNCTION(BlueprintCallable, Category = "Inventory")
		TArray<UItemUI*>& GetItems();

	UFUNCTION(BlueprintCallable, Category = "Inventory")
		TArray<UItemUI*>& GetSelectedItems();

	UFUNCTION(BlueprintCallable, Category = "Inventory")
		void SetItems(TArray<UItemUI*> val);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
		void AddItem(UItemUI* item);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
		void RemoveItem(UItemUI* item);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
		void SelectItem(UItemUI* item);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
		void DeselectItem(UItemUI* item);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
		void EquipItems();
};
