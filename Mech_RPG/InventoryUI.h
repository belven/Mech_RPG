// Copyright of Explosive Industries

#pragma once

#include "Blueprint/UserWidget.h"
#include "InventoryUI.generated.h"

class AMech_RPGCharacter;

UCLASS()
class MECH_RPG_API UInventoryUI : public UUserWidget
{
	GENERATED_BODY()
private:
	AMech_RPGCharacter* owner;

public:
	void SetOwner(AMech_RPGCharacter* inOwner);
	AMech_RPGCharacter* GetOwner();
	
	
};
