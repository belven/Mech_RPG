// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "Armour.h"
#include "Characters/Mech_RPGCharacter.h"

float AArmour::GetResistance(EDamageType damageType) {
	switch (damageType) {
	case EDamageType::Physical:
		return physicalResistance;
	case EDamageType::Energy:
		return energyResistance;
	case EDamageType::Blast:
		return blastResistance;
	}
	return 0;
}

FString AArmour::GetPositionName(TEnumAsByte<ArmourEnums::ArmourPosition> pos) {
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

float AArmour::GetDeafultValue(ArmourGrades::ArmourGrade armourGrade) {
	switch (armourGrade) {
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

FString AArmour::GetTooltipText()
{
	FString output = GetName() + UMiscLibrary::lnBreak;
	output += FindObject<UEnum>(ANY_PACKAGE, TEXT("ArmourEnums"), true)->GetDisplayNameTextByValue(GetArmourPosition()).ToString() + UMiscLibrary::lnBreak;
	output += "Physical Resistance: " + FString::SanitizeFloat(GetPhysicalResistance()) + UMiscLibrary::lnBreak;
	output += "Blast Resistance: " + FString::SanitizeFloat(GetBlastResistance()) + UMiscLibrary::lnBreak;
	output += "Energy Resistance: " + FString::SanitizeFloat(GetEnergyResistance());
	return output;
}

AItem* AArmour::Copy()
{
	AArmour* armour = CreateArmour(GetWorld(), GetName(), physicalResistance, 
		blastResistance, energyResistance, armourPosition, GetItemOwner(), GetGrade(), GetQuality());
	armour->CloneItemSettings(this);
	return armour;
}

float AArmour::GetPhysicalResistance() {
	return physicalResistance;
}

float AArmour::GetBlastResistance() {
	return blastResistance;
}

float AArmour::GetEnergyResistance() {
	return energyResistance;
}

TEnumAsByte<ArmourEnums::ArmourPosition> AArmour::GetArmourPosition() {
	return armourPosition;
}

AArmour* AArmour::CreateArmour(UWorld* world,
	FString armourName,
	float inPhysicalResistance,
	float inBlastResistance,
	float inEnergyResistance,
	ArmourEnums::ArmourPosition inArmourPosition,
	AMech_RPGCharacter* armourOwner,
	int32 armourGrade,
	int32 armourQuality)
{
	FActorSpawnParameters params;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AArmour* tempArmour = world->SpawnActor<AArmour>(StaticClass(), params);
	tempArmour->armourPosition = inArmourPosition;
	tempArmour->physicalResistance = inPhysicalResistance;
	tempArmour->blastResistance = inBlastResistance;
	tempArmour->energyResistance = inEnergyResistance;
	tempArmour->SetGrade(armourGrade);
	tempArmour->SetQuality(armourQuality);
	tempArmour->SetItemOwner(armourOwner);
	tempArmour->SetName(armourName);
	
	if (armourOwner != nullptr) {
		tempArmour->AttachToComponent(armourOwner->GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false));
	}

	return tempArmour;
}

AArmour* AArmour::CreateArmour(UWorld* world,
	FString armourName,
	float inResistance,
	ArmourEnums::ArmourPosition inArmourPosition,
	AMech_RPGCharacter* armourOwner,
	int32 armourGrade,
	int32 armourQuality)
{
	return CreateArmour(world, armourName, inResistance, inResistance, inResistance, inArmourPosition, armourOwner, armourGrade, armourQuality);
}
