#pragma once

UENUM(BlueprintType)
enum class EAffectedTeam : uint8
{
	Ally,
	Enemy,
	Both
};

UENUM(BlueprintType)
namespace ArmourEnums
{
	enum ArmourPosition
	{
		Left_Leg,
		Right_Leg,
		Chest,
		Left_Arm,
		Right_Arm,
		End
	};
}

UENUM(BlueprintType)
namespace ArmourGrades
{
	enum ArmourGrade
	{
		Light,
		MediumLight,
		Medium,
		MediumHeavy,
		Heavy,
		End
	};
}

UENUM(BlueprintType)
enum class EDamageType : uint8
{
	Physical,
	Blast,
	Energy
};

UENUM(BlueprintType)
namespace GameEnums
{
	enum Difficulty
	{
		Easy,
		Medium,
		Hard
	};
}

UENUM(BlueprintType)
namespace WeaponEnums
{
	enum WeaponType
	{
		Sniper,
		Bio_Repair,
		SMG,
		Shotgun,
		RPG,
		Sword,
		End
	};
}