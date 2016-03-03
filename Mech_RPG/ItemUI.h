// Copyright of Explosive Industries

#pragma once

#include "Blueprint/UserWidget.h"
#include "ItemUI.generated.h"

class AItem;

UCLASS()
class MECH_RPG_API UItemUI : public UUserWidget
{
	GENERATED_BODY()
private:
	AItem* owner;

public:
	void SetItem(AItem* inItem);
	AItem* GetItem();
	
	
	
	
};
