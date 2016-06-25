// Copyright of Explosive Industries

#pragma once

#include "MechPart.h"
#include "Equipable.generated.h"

/**
 * 
 */
UCLASS()
class MECH_RPG_API AEquipable : public AMechPart
{
	GENERATED_BODY()
private:
	bool equipped = false;

public:
	virtual void Equip();

	virtual void Unequip();
	
	
	bool IsEquipped() const { return equipped; }
	void SetEquipped(bool val) { equipped = val; }
};
