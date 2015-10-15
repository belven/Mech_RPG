#pragma once
#include "Mech_RPG.h"
#include "Boss.h"

TEnumAsByte<BossEnums::BossRole> ABoss::GetRandomRole() {
	return (BossEnums::BossRole)(UMiscLibrary::GetRandomEnum(BossEnums::End));
}

void ABoss::CreatePresetRole(TEnumAsByte<BossEnums::BossRole> inRole) {
	FWeaponParams params;

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
		params.damage = 400;
		params.fireRate = 1;
		params.range = 1300;

		AddWeapon(AWeapon::CreateWeapon(this, params));
		AddAbility(UDamageBoost::CreateAbility(7, this, 1.5));
		SetDefenceModifier(0 + statModifier);
		SetDamageModifier(1.2 + statModifier);
		break;
	default:
		CreatePresetRole(BossEnums::DPS);
		break;
	}
}