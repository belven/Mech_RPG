#pragma once
#include "Mech_RPG.h"
#include "Boss.h"
#include "Weapons.h"
#include "Abilities.h"
#include "Delayed Events/PhaseTrigger.h"

TEnumAsByte<GroupEnums::Role> ABoss::GetRandomRole() {
	return (GroupEnums::Role)(UMiscLibrary::GetRandomEnum(GroupEnums::End));
}

void ABoss::SetPhase(int32 val)
{
	phase = val;
}

void ABoss::CreatePresetRole(TEnumAsByte<GroupEnums::Role> inRole, int32 grade, int32 quaility) {
	float blastResistance = 5;
	float phsyicalResistance = 5;
	float energyResistance = 5;
	int32 newQuality = quaility;

	StartingRole(inRole);
	Reset();

	switch (inRole) {
	case GroupEnums::DPS:
		SetCurrentWeapon(mCreatePresetWeapon(WeaponEnums::SMG, grade, newQuality));
		AddAbility(UDamageBoost::CreateAbility(7, this, 0.5));
		AddAbility(UChannelledAbility::CreateChannelledAbility(this, UParticleBomb::CreateAbility(12.0F, this, 20.0F), 5.0F, false, true));
		SetMaxHealth(lowHealth);
		energyResistance = mGetDefaultArmourValue(ArmourGrades::Medium);
		phsyicalResistance = mGetDefaultArmourValue(ArmourGrades::MediumLight);
		blastResistance = mGetDefaultArmourValue(ArmourGrades::Medium);
		break;
	case GroupEnums::Tank:
		SetCurrentWeapon(mCreatePresetWeapon(WeaponEnums::Sword, grade, newQuality));
		AddAbility(UAbility::CreatePresetAbility(this, AbilityEnums::Taunt));
		AddAbility(UAbility::CreateChannelledPresetAbility(this, AbilityEnums::Stun, 2.5, false, true));
		SetMaxHealth(highHealth);
		energyResistance = mGetDefaultArmourValue(ArmourGrades::Heavy);
		phsyicalResistance = mGetDefaultArmourValue(ArmourGrades::Heavy);
		blastResistance = mGetDefaultArmourValue(ArmourGrades::Heavy);
		break;
	case GroupEnums::Sniper:
		SetCurrentWeapon(mCreatePresetWeapon(WeaponEnums::Sniper, grade, newQuality));
		AddAbility(UChannelledAbility::CreateChannelledAbility(this, USnipe::CreateAbility(7, this), 2.5F, false, true));
		SetMaxHealth(lowHealth);
		energyResistance = mGetDefaultArmourValue(ArmourGrades::Medium);
		phsyicalResistance = mGetDefaultArmourValue(ArmourGrades::Light);
		blastResistance = mGetDefaultArmourValue(ArmourGrades::Medium);
		break;
	case GroupEnums::Healer:
		SetCurrentWeapon(mCreatePresetWeapon(WeaponEnums::Bio_Repair, grade, newQuality));
		AddAbility(UHeal::CreateAbility(5, this, 12.0F));
		AddAbility(UDisable::CreateDisable(10, this, 5));
		SetMaxHealth(mediumHealth);
		energyResistance = mGetDefaultArmourValue(ArmourGrades::Medium);
		phsyicalResistance = mGetDefaultArmourValue(ArmourGrades::MediumHeavy);
		blastResistance = mGetDefaultArmourValue(ArmourGrades::Medium);
		break;
	default:
		CreatePresetRole(GroupEnums::DPS);
		break;
	}
	SetHealth(GetMaxHealth());

	CreateArmour(phsyicalResistance, blastResistance, energyResistance, grade, quaility);
}