#pragma once
#include "Mech_RPG.h"
#include "Boss.h"

TEnumAsByte<GroupEnums::Role> ABoss::GetRandomRole() {
	return (GroupEnums::Role)(UMiscLibrary::GetRandomEnum(GroupEnums::End));
}

void ABoss::CreatePresetRole(TEnumAsByte<GroupEnums::Role> inRole , int32 grade , int32 quaility) {
	FMagazineWeaponParams params;
	float statModifier = 0;
	int armourValue = 5;

	Reset();

	switch (UMiscLibrary::GetDifficulty()) {
	case GameEnums::Easy:
		GetModifierForDifficulty(GameEnums::Hard);
		break;
	case GameEnums::Medium:
		GetModifierForDifficulty(GameEnums::Medium);
		break;
	case GameEnums::Hard:
		GetModifierForDifficulty(GameEnums::Easy);
		break;
	}

	switch (inRole) {
	case GroupEnums::DPS:
		params.healthChange = 100;
		params.fireRate = 0.3;
		params.range = 1300;
		params.magazineSize = 40;
		params.reloadAmount = 4;
		params.heals = false;

		AddWeapon(AMagazineWeapon::CreateMagazineWeapon(this, params));
		AddAbility(UDamageBoost::CreateAbility(7, this, 0.5));
		AddAbility(UChannelledAbility::CreateChannelledAbility(this, UTimedHealthChange::CreateTimedHealthChange(this, 10.0F, 400.0F), 2, true, true));
		SetDefenceModifier(0 + statModifier);
		SetHealthChangeModifier(1 + statModifier);
		SetMaxHealth(3000 * (1 + statModifier));
		armourValue = UArmour::GetDeafultValue(ArmourGrades::Light);
		break;
	case GroupEnums::Tank:
		params.healthChange = 1200;
		params.fireRate = 1;
		params.range = 400;

		AddWeapon(AWeapon::CreateWeapon(this, params));
		AddAbility(UTaunt::CreateAbility(5, this));
		AddAbility(UChannelledAbility::CreateChannelledAbility(this, UImmobilise::CreateAbility(10, this, 5), 2.5, true, true));
		SetDefenceModifier(0 + statModifier);
		SetHealthChangeModifier(1 + statModifier);
		SetMaxHealth(3000 * (1 + statModifier));
		armourValue = UArmour::GetDeafultValue(ArmourGrades::Heavy);
		break;
	case GroupEnums::Sniper:
		params.healthChange = 800;
		params.fireRate = 2.5;
		params.range = 2000;
		params.magazineSize = 5;
		params.reloadAmount = 1;
		params.heals = false;

		AddWeapon(AMagazineWeapon::CreateMagazineWeapon(this, params));
		AddAbility(USnipe::CreateAbility(7, this));
		SetDefenceModifier(0 + statModifier);
		SetHealthChangeModifier(1 + statModifier);
		SetMaxHealth(3000 * (1 + statModifier));
		armourValue = UArmour::GetDeafultValue(ArmourGrades::Heavy);
		break;
	case GroupEnums::Healer:
		AddWeapon(AWeapon::CreatePresetWeapon(this, WeaponEnums::Bio_Repair));
		GetWeapons()[0]->SetChangeAmount(GetWeapons()[0]->GetChangeAmount() * 1.4);
		AddAbility(UHeal::CreateAbility(5, this, 800));
		AddAbility(UDisable::CreateDisable(10, this, 5));
		SetDefenceModifier(0 + statModifier);
		SetHealthChangeModifier(1 + statModifier);
		SetMaxHealth(3000 * (1 + statModifier));
		armourValue = UArmour::GetDeafultValue(ArmourGrades::Medium);
		break;
	default:
		CreatePresetRole(GroupEnums::DPS);
		break;
	}
	SetHealth(GetMaxHealth());

	for (int i = 0; i < ArmourEnums::End; i++) {
		GetArmour().Add(UArmour::CreateArmour(armourValue, armourValue, armourValue, (ArmourEnums::ArmourPosition)i));
	}
}