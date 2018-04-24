// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "Array.h"
#include "Item.h"
#include "SlotContainer.generated.h"

/**
 *
 */
UCLASS(Blueprintable)
class USlotContainer : public UObject
{
	GENERATED_BODY()

public:
	USlotContainer();
	UFUNCTION(BlueprintCallable, Category = "Slot Container")
		TArray<UItem*>& GetItems();

	UFUNCTION(BlueprintCallable, Category = "Slot Container")
		void SetItems(TArray<UItem*> newVal);

	UItem * GetExistingItemWithSpace(UItem* inItem);

	UFUNCTION(BlueprintCallable, Category = "Slot Container")
		UItem* AddItem(UItem* itemToAdd);

	UFUNCTION(BlueprintCallable, Category = "Slot Container")
		bool RemoveItem(UItem* itemToRemove);

	UFUNCTION(BlueprintCallable, Category = "Slot Container")
		int32 GetItemAmount(FString name);

	UFUNCTION(BlueprintCallable, Category = "Slot Container")
		int32 GetMaxItemCount();

	UFUNCTION(BlueprintCallable, Category = "Slot Container")
		void SetMaxItemCount(int32 newVal);

	UFUNCTION(BlueprintCallable, Category = "Slot Container")
		bool HasSpace();

private:
	UPROPERTY()
		TArray<UItem*> items;

	UPROPERTY()
		int32 maxItemCount = 20;

};
