// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "Weapons/Weapon.h"
#include "Abilities/OrbitalStrike.h"
#include "Abilities/ChannelledAbility.h"
#include "MiscLibrary.h"
#include "TestBoss.h"
#include "Abilities/Immobilise.h"
#include "HealthChangePhaseTrigger.h"

#define mCreateHealthChangePhaseTrigger(health, phase) UHealthChangePhaseTrigger::CreateHealthChangePhaseTrigger(this, health, phase)
#define mGetMeleeRange UMiscLibrary::GetMeleeRange(this)

void ATestBoss::CreatePresetRole(TEnumAsByte<GroupEnums::Role> inRole, int32 grade, int32 quaility)
{
	float statModifier = 0;
	float blastResistance = 5;
	float phsyicalResistance = 5;
	float energyResistance = 5;
	static float Health = 20000;

	StartingRole(inRole);

	Reset();
	ApplyCrowdControl(EffectEnums::Cast, false); //Default to not casting

	FWeaponParams params;
	params.critChance = 0;
	params.damageType = DamageEnums::Blast;
	params.fireRate = 5;
	params.heals = false;
	params.grade = grade;
	params.quality = (QualityEnums::Quality)quaility;
	params.range = 5000;
	params.healthChange = 1500;

	SetCurrentWeapon(AWeapon::CreateWeapon(GetWorld(), this, params));
	AddAbility(mCreateChannelledAbility(UOrbitalStrike::CreateAbility(0.1, this, 2.5F), 6, true, false));
	SetDefenceModifier(0.0F + statModifier);
	SetHealthChangeModifier(1.0F + statModifier);
	blastResistance = AArmour::GetDeafultValue(ArmourGrades::Medium);
	phsyicalResistance = AArmour::GetDeafultValue(ArmourGrades::MediumHeavy);
	energyResistance = AArmour::GetDeafultValue(ArmourGrades::Medium);
	SetMaxHealth(Health * (1 + statModifier));

	SetHealth(GetMaxHealth());

	CreateArmour(phsyicalResistance, blastResistance, energyResistance, grade, quaility);

	triggers.Add(mCreateHealthChangePhaseTrigger(0.15, 1));
	triggers.Add(mCreateHealthChangePhaseTrigger(0.15, 2));
	triggers.Add(mCreateHealthChangePhaseTrigger(0.15, 3));
}

void ATestBoss::SetPhase(int32 val)
{
	Super::SetPhase(val);

	if (GetPhase() == 1) {
		ApplyCrowdControl(EffectEnums::Cast, true); // Set can cast
		ApplyCrowdControl(EffectEnums::Attack, false);
		SetHealth(GetMaxHealth());
	}
	else if (GetPhase() == 2) {
		ApplyCrowdControl(EffectEnums::Cast, false);
		ApplyCrowdControl(EffectEnums::Attack, true);
		SetHealth(GetMaxHealth());
	}
	else if (GetPhase() == 3) {
		FWeaponParams params;
		params.critChance = 40;
		params.damageType = DamageEnums::Physical;
		params.fireRate = 1;
		params.grade = GetCurrentWeapon()->GetGrade();
		params.quality = (QualityEnums::Quality)GetCurrentWeapon()->GetQuality();
		params.range = mGetMeleeRange;
		params.healthChange = 1750;

		SetCurrentWeapon(AWeapon::CreateWeapon(GetWorld(), this, params));

		GetAbilities().Empty();
		AddAbility(mCreateChannelledAbility(UImmobilise::CreateAbility(10, this, 5), 3, false, true));

		ApplyCrowdControl(EffectEnums::Cast, true);
		SetHealth(GetMaxHealth());
		SetSpeedModifier(0.8);
		SetDefenceModifier(1.3);
	}
}