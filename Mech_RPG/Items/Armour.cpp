// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "Armour.h"
#include "Enums.h"
#include "Characters/Mech_RPGCharacter.h"

float UArmour::GetResistance(EDamageType damageType)
{
	switch (damageType)
	{
	case EDamageType::Physical:
		return physicalResistance;
	case EDamageType::Energy:
		return energyResistance;
	case EDamageType::Blast:
		return blastResistance;
	}
	return 0;
}

FString UArmour::GetPositionName(EArmourPosition pos)
{
	switch (pos)
	{
	case EArmourPosition::Chest:
		return "Chest";
	case EArmourPosition::Right_Arm:
		return "Right Arm";
	case EArmourPosition::Right_Leg:
		return "Right Leg";
	case EArmourPosition::Left_Arm:
		return "Left Arm";
	case EArmourPosition::Left_Leg:
		return "Left Leg";
	}
	return "Not Found";
}

float UArmour::GetDeafultValue(ArmourGrades::ArmourGrade armourGrade)
{
	switch (armourGrade)
	{
	case ArmourGrades::Light:
		return 4.0F;
	case ArmourGrades::MediumLight:
		return 6.0F;
	case ArmourGrades::Medium:
		return 8.0F;
	case ArmourGrades::MediumHeavy:
		return 10.0F;
	case ArmourGrades::Heavy:
		return 12.0F;
	default:
		return 0.0F;
	}
}

FString UArmour::GetTooltipText()
{
	FString output = GetName() + UMiscLibrary::lnBreak;
	output += FindObject<UEnum>(ANY_PACKAGE, TEXT("EArmourEnum"), true)->GetDisplayNameTextByValue((int)GetArmourPosition()).ToString() + UMiscLibrary::lnBreak;
	output += "Physical Resistance: " + FString::SanitizeFloat(GetPhysicalResistance()) + UMiscLibrary::lnBreak;
	output += "Blast Resistance: " + FString::SanitizeFloat(GetBlastResistance()) + UMiscLibrary::lnBreak;
	output += "Energy Resistance: " + FString::SanitizeFloat(GetEnergyResistance());
	return output;
}

UItem* UArmour::Copy()
{
	UArmour* armour = CreateArmour(GetName(), physicalResistance,
		blastResistance, energyResistance, armourPosition, GetItemOwner(), GetGrade(), GetQuality());
	armour->CloneItemSettings(this);
	return armour;
}

float UArmour::GetPhysicalResistance()
{
	return physicalResistance;
}

float UArmour::GetBlastResistance()
{
	return blastResistance;
}

float UArmour::GetEnergyResistance()
{
	return energyResistance;
}

EArmourPosition UArmour::GetArmourPosition()
{
	return armourPosition;
}

UArmour* UArmour::CreateArmour(FString armourName,
	float inPhysicalResistance,
	float inBlastResistance,
	float inEnergyResistance,
	EArmourPosition inArmourPosition,
	AMech_RPGCharacter* armourOwner,
	int32 armourGrade,
	int32 armourQuality)
{
	UArmour* tempArmour = NewObject<UArmour>(StaticClass());
	tempArmour->armourPosition = inArmourPosition;
	tempArmour->physicalResistance = inPhysicalResistance;
	tempArmour->blastResistance = inBlastResistance;
	tempArmour->energyResistance = inEnergyResistance;
	tempArmour->SetGrade(armourGrade);
	tempArmour->SetQuality(armourQuality);
	tempArmour->SetItemOwner(armourOwner);
	tempArmour->SetName(armourName);
	return tempArmour;
}

UArmour* UArmour::CreateArmour(
	FString armourName,
	float inResistance,
	EArmourPosition inArmourPosition,
	AMech_RPGCharacter* armourOwner,
	int32 armourGrade,
	int32 armourQuality)
{
	return CreateArmour(armourName, inResistance, inResistance, inResistance, inArmourPosition, armourOwner, armourGrade, armourQuality);
}
