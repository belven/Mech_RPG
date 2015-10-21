// Copyright of Explosive Industries

UENUM(BlueprintType)
namespace ArmourEnums {
	enum ArmourPosition {
		Left_Leg,
		Right_Leg,
		Chest,
		Left_Arm,
		Right_Arm,
		Other
	};
}

namespace DamageEnums {
	enum DamageType {
		Physical,
		Blast,
		Energy
	};
}
#pragma once

#include "Object.h"
#include "Armour.generated.h"

UCLASS()
class MECH_RPG_API UArmour : public UObject
{
	GENERATED_BODY()
private:
	TEnumAsByte<ArmourEnums::ArmourPosition> armourPosition;

	float physicalResistance;
	float blastResistance;
	float energyResistance;

public:
	
	float GetResistance(DamageEnums::DamageType damageType);
	
	static UArmour* CreateArmour(float inPhysicalResistance, float inBlastResistance, float inEnergyResistance, ArmourEnums::ArmourPosition inArmourPosition);
};
