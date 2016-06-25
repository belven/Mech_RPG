// Fill out your copyright notice in the Description page of Project Settings.

UENUM(BlueprintType)
namespace BossEnums {
	enum BossRole {
		None,
		DPS,
		Healer,
		Tank,
		Sniper,
		End
	};
}

#pragma once
#include "Characters/Mech_RPGCharacter.h"
#include "Boss.generated.h"


UCLASS(Blueprintable)
class MECH_RPG_API ABoss : public AMech_RPGCharacter
{
	GENERATED_BODY()
private:

public:
	void CreatePresetRole(TEnumAsByte<GroupEnums::Role> inRole = GroupEnums::DPS, int32 grade = 0, int32 quaility = 0) override;

	static TEnumAsByte<GroupEnums::Role> GetRandomRole();
};
