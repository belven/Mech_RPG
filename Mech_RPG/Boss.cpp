#pragma once
#include "Mech_RPG.h"
#include "Boss.h"
#include "Weapons.h"
#include "Abilities.h"

TEnumAsByte<GroupEnums::Role> ABoss::GetRandomRole() {
	return (GroupEnums::Role)(UMiscLibrary::GetRandomEnum(GroupEnums::End));
}

void ABoss::CreatePresetRole(TEnumAsByte<GroupEnums::Role> inRole, int32 grade, int32 quaility) {
	float statModifier = 0;
	float blastResistance = 5;
	float phsyicalResistance = 5;
	float energyResistance = 5;
	static float lowHealth = 8000;
	static float mediumHealth = lowHealth * 1.25;
	static float highHealth = mediumHealth * 1.25;

	StartingRole(inRole);
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
		SetCurrentWeapon(mCreatePresetWeapon(WeaponEnums::SMG, grade, quaility + 3));
		AddAbility(UDamageBoost::CreateAbility(7, this, 0.5));
		//AddAbility(UChannelledAbility::CreateChannelledAbility(this, UTimedHealthChange::CreateTimedHealthChange(this, 10.0F, 400.0F), 2, true, true));
		//AddAbility(UOrbitalStrike::CreateAbility(30, this, 350));
		AddAbility(UChannelledAbility::CreateChannelledAbility(this, UParticleBomb::CreateAbility(30.0F, this, 0.6F), 4.5F, false, true));
		SetDefenceModifier(0 + statModifier);
		SetHealthChangeModifier(1 + statModifier);
		SetMaxHealth(lowHealth * (1 + statModifier));
		energyResistance = AArmour::GetDeafultValue(ArmourGrades::Medium);
		phsyicalResistance = AArmour::GetDeafultValue(ArmourGrades::MediumLight);
		blastResistance = AArmour::GetDeafultValue(ArmourGrades::Medium);
		break;
	case GroupEnums::Tank:
		SetCurrentWeapon(mCreatePresetWeapon(WeaponEnums::Sword, grade, quaility + 3));
		AddAbility(UAbility::CreatePresetAbility(this, AbilityEnums::Taunt));
		AddAbility(UAbility::CreateChannelledPresetAbility(this, AbilityEnums::Stun, 2.5, false, true));
		SetDefenceModifier(0 + statModifier);
		SetHealthChangeModifier(1 + statModifier);
		SetMaxHealth(highHealth * (1 + statModifier));
		energyResistance = AArmour::GetDeafultValue(ArmourGrades::Heavy);
		phsyicalResistance = AArmour::GetDeafultValue(ArmourGrades::Heavy);
		blastResistance = AArmour::GetDeafultValue(ArmourGrades::Heavy);
		break;
	case GroupEnums::Sniper:
		SetCurrentWeapon(mCreatePresetWeapon(WeaponEnums::Sniper, grade, quaility + 3));
		AddAbility(USnipe::CreateAbility(7, this));
		SetDefenceModifier(0 + statModifier);
		SetHealthChangeModifier(1 + statModifier);
		SetMaxHealth(lowHealth * (1 + statModifier));
		energyResistance = AArmour::GetDeafultValue(ArmourGrades::Medium);
		phsyicalResistance = AArmour::GetDeafultValue(ArmourGrades::Light);
		blastResistance = AArmour::GetDeafultValue(ArmourGrades::Medium);
		break;
	case GroupEnums::Healer:
		SetCurrentWeapon(mCreatePresetWeapon(WeaponEnums::Bio_Repair, grade, quaility + 3));
		AddAbility(UHeal::CreateAbility(5, this, 800));
		AddAbility(UDisable::CreateDisable(10, this, 5));
		SetDefenceModifier(0 + statModifier);
		SetHealthChangeModifier(1.5 + statModifier);
		SetMaxHealth(mediumHealth * (1 + statModifier));
		energyResistance = AArmour::GetDeafultValue(ArmourGrades::Medium);
		phsyicalResistance = AArmour::GetDeafultValue(ArmourGrades::MediumHeavy);
		blastResistance = AArmour::GetDeafultValue(ArmourGrades::Medium);
		break;
	default:
		CreatePresetRole(GroupEnums::DPS);
		break;
	}
	SetHealth(GetMaxHealth());

	for (int i = 0; i < ArmourEnums::End; i++) {
		AArmour* newArmour = AArmour::CreateArmour(GetWorld(), "Test", phsyicalResistance, blastResistance, energyResistance, (ArmourEnums::ArmourPosition)i, this, grade, quaility);
		GetArmour().Add(newArmour);
		AddItem(newArmour);
	}
}