#pragma once
#include "Mech_RPG.h"
#include "Boss.h"

TEnumAsByte<BossEnums::BossRole> ABoss::GetRandomRole() {
	return (BossEnums::BossRole)(UMiscLibrary::GetRandomEnum(BossEnums::End));
}

void ABoss::CreatePresetRole(TEnumAsByte<BossEnums::BossRole> inRole) {
	int armourValue = 5;
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
		params.damage = 100;
		params.fireRate = 0.3;
		params.range = 1300;
		params.magazineSize = 40;
		params.reloadAmount = 4;
		params.heals = false;

		AddWeapon(AMagazineWeapon::CreateMagazineWeapon(this, params));
		AddAbility(UDamageBoost::CreateAbility(7, this, 0.5));
		AddAbility(UChannelledAbility::CreateChannelledAbility(this, UTimedHealthChange::CreateTimedHealthChange(this, 10.0F, 400.0F), 2, true, true));
		SetDefenceModifier(0 + statModifier);
		SetDamageModifier(1 + statModifier);
		armourValue = 5;
		break;
	case BossEnums::Tank:
		params.damage = 1200;
		params.fireRate = 1;
		params.range = 400;

		AddWeapon(AWeapon::CreateWeapon(this, params));
		AddAbility(UTaunt::CreateAbility(5, this));
		AddAbility(UChannelledAbility::CreateChannelledAbility(this, UImmobilise::CreateAbility(10, this, 5), 2.5, true, true));
		SetDefenceModifier(0 + statModifier);
		SetDamageModifier(1 + statModifier);
		armourValue = 10;
		break;
	case BossEnums::Sniper:
		params.damage = 800;
		params.fireRate = 2.5;
		params.range = 2000;
		params.magazineSize = 5;
		params.reloadAmount = 1;
		params.heals = false;

		AddWeapon(AMagazineWeapon::CreateMagazineWeapon(this, params));
		AddAbility(USnipe::CreateAbility(7, this));
		SetDefenceModifier(0 + statModifier);
		SetDamageModifier(1 + statModifier);
		armourValue = 5;
		break;
	default:
		CreatePresetRole(BossEnums::DPS);
		break;
	}

	GetArmour().Empty();

	for (int i = 0; i < ArmourEnums::End; i++) {
		GetArmour().Add(UArmour::CreateArmour(armourValue, armourValue, armourValue, (ArmourEnums::ArmourPosition)i));
	}
}