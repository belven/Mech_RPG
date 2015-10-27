// Copyright of Explosive Industries

#include "Mech_RPG.h"

float UArmour::GetResistance(DamageEnums::DamageType damageType) {
	switch (damageType) {
	case DamageEnums::Physical:
		return physicalResistance;
	case DamageEnums::Energy:
		return energyResistance;
	case DamageEnums::Blast:
		return blastResistance;
	}
	return 0;
}

FString UArmour::GetPositionName(TEnumAsByte<ArmourEnums::ArmourPosition> pos) {
	switch (pos) {
	case ArmourEnums::Chest:
		return "Chest";
	case ArmourEnums::Right_Arm:
		return "Right Arm";
	case ArmourEnums::Right_Leg:
		return "Right Leg";
	case ArmourEnums::Left_Arm:
		return "Left Arm";
	case ArmourEnums::Left_Leg:
		return "Left Leg";
	}
	return "Not Found";
}

float UArmour::GetPhysicalResistance() {
	return physicalResistance;
}

float UArmour::GetBlastResistance() {
	return blastResistance;
}

float UArmour::GetEnergyResistance() {
	return energyResistance;
}

TEnumAsByte<ArmourEnums::ArmourPosition> UArmour::GetArmourPosition() {
	return armourPosition;
}

UArmour* UArmour::CreateArmour(float inPhysicalResistance, float inBlastResistance, float inEnergyResistance, ArmourEnums::ArmourPosition inArmourPosition) {
	UArmour* tempArmour = NewObject<UArmour>(StaticClass());
	tempArmour->armourPosition = inArmourPosition;
	tempArmour->physicalResistance = inPhysicalResistance;
	tempArmour->blastResistance = inBlastResistance;
	tempArmour->energyResistance = inEnergyResistance;
	return tempArmour;
}