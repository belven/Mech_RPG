#pragma once
#include "Mech_RPG.h"
#include "Boss.h"



void ABoss::CreatePresetRole(TEnumAsByte<BossEnums::BossRole> inRole) {
	SetHealth(GetMaxHealth());
	SetHealthRegen(10.0);

	GameEnums::Difficulty difficulty = GameEnums::Easy;

	float statModifier = 0;

	switch (difficulty) {
	case GameEnums::Easy:
		statModifier = 0.2;
		break;
	case GameEnums::Medium:
		statModifier = 0.3;
		break;
	case GameEnums::Hard:
		statModifier = 0.4;
		break;
	}

	switch (inRole) {
	case BossEnums::DPS:
		AddWeapon(AWeapon::CreatePresetWeapon(this, WeaponEnums::SMG));
		AddAbility(UChannelledAbility::CreateChannelledAbility(this, UGrenade::CreateAbility(7, this, 0.2), 1.5));
		SetDefenceModifier(0 + statModifier);
		SetDamageModifier(1.5 + statModifier);
		break;
	default:
		CreatePresetRole(BossEnums::DPS);
		break;
	}
}