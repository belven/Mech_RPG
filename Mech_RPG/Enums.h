#pragma once

UENUM(BlueprintType)
enum class EAffectedTeam : uint8
{
	Ally,
	Enemy,
	Both
}; 

UENUM(BlueprintType)
enum class  ERole : uint8
{
	None,
	DPS,
	Drone,
	Healer,
	RPG,
	Sniper,
	Support,
	Tank,
	End
};

UENUM(BlueprintType)
enum class ESpecialisation : uint8
{
	Pacifier,
	Havoc,
	Defiler,
	Invigorator
};

UENUM(BlueprintType)
enum class EStatEnum : uint8
{
	Health,
	Damage,
	Defence,
	MovementSpeed,
	CastSpeed,
	AttackSpeed,
	CritChance
};

UENUM(BlueprintType)
enum class EArmourPosition : uint8
{
	Left_Leg,
	Right_Leg,
	Chest,
	Left_Arm,
	Right_Arm,
	End
};

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
		Drone_Weapon,
		SMG,
		Shotgun,
		RPG,
		Sword,
		End
	};
}