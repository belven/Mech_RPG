// Copyright of Explosive Industries
#pragma once

#include "Object.h"
#include "Item.h"
#include "Armour.generated.h"

UENUM(BlueprintType)
namespace ArmourEnums {
	enum ArmourPosition {
		Left_Leg,
		Right_Leg,
		Chest,
		Left_Arm,
		Right_Arm,
		End
	};
}

UENUM(BlueprintType)
namespace ArmourGrades {
	enum ArmourGrade {
		Light,
		MediumLight,
		Medium,
		MediumHeavy,
		Heavy,
		End
	};
}

UENUM(BlueprintType)
enum class EDamageType : uint8 {
	Physical,
	Blast,
	Energy
};

UCLASS(BlueprintType)
class MECH_RPG_API AArmour : public AItem {
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

	virtual AItem* Copy() override;

	static AArmour* CreateArmour(UWorld* world,
		FString armourName, float inResistance, ArmourEnums::ArmourPosition inArmourPosition, AMech_RPGCharacter* armourOwner = nullptr, int32 armourGrade = 1, int32 armourQuality = 1);

	static AArmour* CreateArmour(UWorld* world,
		FString armourName, float inPhysicalResistance, float inBlastResistance, float inEnergyResistance, ArmourEnums::ArmourPosition inArmourPosition, AMech_RPGCharacter* armourOwner = nullptr, int32 armourGrade = 1, int32 armourQuality = 1);
};
