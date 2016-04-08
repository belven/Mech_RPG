// Copyright of Explosive Industries

#pragma once

#include "Blueprint/UserWidget.h"
#include "InventoryUI.generated.h"

class AMech_RPGCharacter;

UCLASS()
class MECH_RPG_API UInventoryUI : public UUserWidget
{
	GENERATED_BODY()
protected:
	AMech_RPGCharacter* owner;

public:
	UFUNCTION(BlueprintCallable, Category = "Inventory")
		void SetOwner(AMech_RPGCharacter* inOwner);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
		AMech_RPGCharacter* GetOwner();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Quests")
		void GenerateInventory();


};
