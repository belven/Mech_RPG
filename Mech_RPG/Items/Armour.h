// Copyright of Explosive Industries
#pragma once

#include "Object.h"
#include "Item.h"
#include "Enums.h"
#include "Armour.generated.h"

UCLASS(BlueprintType)
class MECH_RPG_API UArmour : public UItem
{
	GENERATED_BODY()
private:
	TEnumAsByte<ArmourEnums::ArmourPosition> armourPosition;

	float physicalResistance = 0.15;
	float blastResistance = 0.15;
	float energyResistance = 0.15;

public:

	UFUNCTION(BlueprintCallable, Category = "Armour")
		TEnumAsByte<ArmourEnums::ArmourPosition> GetArmourPosition();

	UFUNCTION(BlueprintCallable, Category = "Armour")
		float GetPhysicalResistance();

	UFUNCTION(BlueprintCallable, Category = "Armour")
		float GetBlastResistance();

	UFUNCTION(BlueprintCallable, Category = "Armour")
		float GetEnergyResistance();

	UFUNCTION(BlueprintCallable, Category = "Armour")
		static FString GetPositionName(TEnumAsByte<ArmourEnums::ArmourPosition> pos);

	UFUNCTION(BlueprintCallable, Category = "Armour")
		float GetResistance(EDamageType damageType);

	UFUNCTION(BlueprintCallable, Category = "Armour")
		static float GetDeafultValue(ArmourGrades::ArmourGrade armourGrade);

	UFUNCTION(BlueprintCallable, Category = "Armour")
		virtual FString GetTooltipText() override;

	virtual UItem* Copy() override;

	static UArmour* CreateArmour(FString armourName, float inResistance, ArmourEnums::ArmourPosition inArmourPosition, AMech_RPGCharacter* armourOwner = nullptr, int32 armourGrade = 1, int32 armourQuality = 1);

	static UArmour* CreateArmour(FString armourName, float inPhysicalResistance, float inBlastResistance, float inEnergyResistance, ArmourEnums::ArmourPosition inArmourPosition, AMech_RPGCharacter* armourOwner = nullptr, int32 armourGrade = 1, int32 armourQuality = 1);
};
