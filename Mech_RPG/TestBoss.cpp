// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "Weapons/Weapon.h"
#include "Abilities/OrbitalStrike.h"
#include "Abilities/ChannelledAbility.h"
#include "MiscLibrary.h"
#include "TestBoss.h"

void ATestBoss::CreatePresetRole(TEnumAsByte<GroupEnums::Role> inRole, int32 grade, int32 quaility)
{
	float statModifier = 0;
	float blastResistance = 5;
	float phsyicalResistance = 5;
	float energyResistance = 5;
	static float Health = 100000;

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
	AddAbility(mCreateChannelledAbility(UOrbitalStrike::CreateAbility(7, this, 6.0F), 6, true, false));
	SetDefenceModifier(0.0F + statModifier);
	SetHealthChangeModifier(1.0F + statModifier);
	blastResistance = AArmour::GetDeafultValue(ArmourGrades::Medium);
	phsyicalResistance = AArmour::GetDeafultValue(ArmourGrades::MediumHeavy);
	energyResistance = AArmour::GetDeafultValue(ArmourGrades::Medium);
	SetMaxHealth(Health * (1 + statModifier));

	SetHealth(GetMaxHealth());

	for (int i = 0; i < ArmourEnums::End; i++) {
		AArmour* newArmour = AArmour::CreateArmour(GetWorld(), "Test", phsyicalResistance, blastResistance, energyResistance, (ArmourEnums::ArmourPosition)i, this, grade, quaility);
		GetArmour().Add(newArmour);
		AddItem(newArmour);
	}
}

void ATestBoss::SetPhase(int32 val)
{ 
	phase = val;

	if (phase == 1) {
		ApplyCrowdControl(EffectEnums::Cast, true); // Set can cast
	}
	else if (phase == 2) {
		ApplyCrowdControl(EffectEnums::Cast, false);
	}
	else if (phase == 3) {
		ApplyCrowdControl(EffectEnums::Cast, true);
	}
}

void ATestBoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (phase == 0 && UMiscLibrary::GetHealthPercent(this) <= 0.75) {
		SetPhase(1);
	}
	else if (phase == 1 && UMiscLibrary::GetMissingHealthPercent(this) <= 0.50) {
		SetPhase(2);
	}
	else if (phase == 2 && UMiscLibrary::GetMissingHealthPercent(this) <= 0.25) {
		SetPhase(3);
	}
}