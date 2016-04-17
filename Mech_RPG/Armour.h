// Copyright of Explosive Industries

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

UCLASS(BlueprintType)
class MECH_RPG_API UArmour : public UObject {
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
		float GetResistance(DamageEnums::DamageType damageType);

	UFUNCTION(BlueprintCallable, Category = "Armour")
		static float GetDeafultValue(ArmourGrades::ArmourGrade grade);

	static UArmour* CreateArmour(float inResistance, ArmourEnums::ArmourPosition inArmourPosition, int32 grade = 1, int32 quality = 1);

	static UArmour* CreateArmour(float inPhysicalResistance, float inBlastResistance, float inEnergyResistance, ArmourEnums::ArmourPosition inArmourPosition, int32 grade = 1, int32 quality = 1);
};
