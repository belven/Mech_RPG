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

UArmour* UArmour::CreateArmour(float inPhysicalResistance, float inBlastResistance, float inEnergyResistance, ArmourEnums::ArmourPosition inArmourPosition) {
	UArmour* tempArmour = NewObject<UArmour>(StaticClass());
	tempArmour->armourPosition = inArmourPosition;
	tempArmour->physicalResistance = inPhysicalResistance;
	tempArmour->blastResistance = inBlastResistance;
	tempArmour->energyResistance = inEnergyResistance;
	return tempArmour;
}