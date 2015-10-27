#pragma once
#include "Mech_RPG.h"
#include "Boss.h"

TEnumAsByte<BossEnums::BossRole> ABoss::GetRandomRole() {
	return (BossEnums::BossRole)(UMiscLibrary::GetRandomEnum(BossEnums::End));
}

void ABoss::CreatePresetRole(TEnumAsByte<BossEnums::BossRole> inRole) {
	FMagazineWeaponParams params;

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
		params.damage = 50;
		params.fireRate = 0.3;
		params.range = 1300;
		params.magazineSize = 20;
		params.reloadAmount = 4;

		AddWeapon(AMagazineWeapon::CreateMagazineWeapon(this, params));
		AddAbility(UDamageBoost::CreateAbility(7, this, 0.5));
		SetDefenceModifier(0 + statModifier);
		SetDamageModifier(1.2 + statModifier);
		break;
	case BossEnums::Tank:
		params.damage = 200;
		params.fireRate = 1;
		params.range = 400;
		params.magazineSize = 5;
		params.reloadAmount = 1;

		AddWeapon(AMagazineWeapon::CreateMagazineWeapon(this, params));
		AddAbility(UTaunt::CreateAbility(7, this));
		SetDefenceModifier(0.5 + statModifier);
		SetDamageModifier(1 + statModifier);
		break;
	case BossEnums::Sniper:
		params.damage = 350;
		params.fireRate = 2.5;
		params.range = 2000;
		params.magazineSize = 5;
		params.reloadAmount = 1;

		AddWeapon(AMagazineWeapon::CreateMagazineWeapon(this, params));
		AddAbility(UTaunt::CreateAbility(7, this));
		SetDefenceModifier(0 + statModifier);
		SetDamageModifier(1 + statModifier);
		break;
	default:
		CreatePresetRole(BossEnums::DPS);
		break;
	}
}