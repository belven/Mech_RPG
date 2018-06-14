// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "ShieldBoss.h"
#include "Abilities/ShieldBomb.h"

#define mGetMeleeRange UMiscLibrary::GetMeleeRange(this)


void AShieldBoss::CreatePresetRole(TEnumAsByte<GroupEnums::Role> inRole /*= GroupEnums::DPS*/, int32 grade /*= 0*/, int32 quaility /*= 0*/)
{
	static float newHealth = 20000;

	StartingRole(inRole);

	ResetCharacter();

	FWeaponParams params;
	params.critChance = 0;
	params.damageType = EDamageType::Blast;
	params.fireRate = 5;
	params.heals = false;
	params.grade = grade;
	params.quality = (QualityEnums::Quality)quaility;
	params.range = mGetMeleeRange;
	params.healthChange = 100;

	SetCurrentWeapon(UWeapon::CreateWeapon(this, params));
	SetHealthChangeModifier(1.0F);
	SetMaxHealth(newHealth);
	SetDefenceModifier(0.0F);
	SetHealth(GetMaxHealth());

	AddAbility(UShieldBomb::CreateShieldBomb(5, this, GetMaxHealth() * 0.3));
	
	CreateArmour(mGetDefaultArmourValue(ArmourGrades::Medium),
		mGetDefaultArmourValue(ArmourGrades::Medium),
		mGetDefaultArmourValue(ArmourGrades::Medium),
		grade,
		quaility);
}

void AShieldBoss::SetPhase(int32 val)
{

}
