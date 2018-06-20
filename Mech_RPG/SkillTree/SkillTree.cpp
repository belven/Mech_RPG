// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "Weapons.h"
#include "Abilities.h"
#include "SkillTree.h"


void USkillTree::GainExpereince(float gainedExperience)
{
	this->experience += gainedExperience;

	UE_LOG(SkillTreeLog, Log, TEXT("%s %d"), *UMiscLibrary::GetEnumString("ESpecialisation", (int32)GetSpec()), GetLevel());
}

int USkillTree::GetLevel()
{
	return round(experience / 200.0);
}

float USkillTree::GetStatBonus(EStatEnum statType)
{
	float total = 0;
	for (USkillTreeNode* node : GetNodes())
	{
		total += node->GetStatBonus(statType);
	}
	return total;
}

void USkillTree::OwnerPreChangedOthersHealth(FHealthChange& healthChange)
{
	float gainedExperience = 0;

	for (USkillTreeNode* node : GetNodes())
	{
		node->OwnerPreChangedOthersHealth(healthChange);
	}
	
	switch (GetSpec())
	{
	case ESpecialisation::Pacifier:
		gainedExperience = CheckForPacifierExperience(healthChange);
		break;
	case ESpecialisation::Havoc:
		gainedExperience = CheckForHavocExperience(healthChange);
		break;
	case ESpecialisation::Invigorator:
		gainedExperience = CheckForInvigoratorExperience(healthChange);
		break;
	case ESpecialisation::Defiler:
		gainedExperience = CheckForDefilerExperience(healthChange);
		break;
	}

	if (gainedExperience > 0) {
		GainExpereince(gainedExperience);
	}
}

float USkillTree::CheckForInvigoratorExperience(FHealthChange &healthChange)
{
	if (healthChange.weaponUsed != nullptr && mCheckWeaponUsed(healthChange, UBio_Rifle))
	{
		return healthChange.changeAmount;
	}
	else if (healthChange.abilityUsed != nullptr
		&& (mIsChildOf(healthChange.abilityUsed, UAoEHeal::StaticClass())
			|| mIsChildOf(healthChange.abilityUsed, UHeal::StaticClass())))
	{
		return healthChange.changeAmount;
	}
	return 0;
}

float USkillTree::CheckForDefilerExperience(FHealthChange &healthChange)
{
	if (healthChange.weaponUsed != nullptr
		&& (mCheckWeaponUsed(healthChange, USniper)
			|| mCheckWeaponUsed(healthChange, ULaserSniper))
		)
	{
		return healthChange.changeAmount;
	}
	else if (healthChange.abilityUsed != nullptr && mCheckAbilityUsed(healthChange, USnipe))
	{
		return healthChange.changeAmount;
	}
	return 0;
}

float USkillTree::CheckForHavocExperience(FHealthChange &healthChange)
{
	if (healthChange.weaponUsed != nullptr && mCheckWeaponUsed(healthChange, USMG))
	{
		return healthChange.changeAmount;
	}
	else if (healthChange.abilityUsed != nullptr && mCheckAbilityUsed(healthChange, UGrenade))
	{
		return healthChange.changeAmount;
	}
	return 0;
}

float USkillTree::CheckForPacifierExperience(FHealthChange &healthChange)
{
	if (healthChange.weaponUsed != nullptr && mCheckWeaponUsed(healthChange, UShotgun))
	{
		return healthChange.changeAmount;
	}
	else if (healthChange.abilityUsed != nullptr && mCheckAbilityUsed(healthChange, UBindLife))
	{
		return healthChange.changeAmount;
	}
	return 0;
}

USkillTree* USkillTree::CreateSkillTree(AMech_RPGCharacter * newOwner, ESpecialisation specialisation)
{
	USkillTree* newTree = NewObject<USkillTree>(StaticClass());

	newTree->SetSpec(specialisation);
	newTree->SetTreeOwner(newOwner);

	switch (specialisation)
	{
	case ESpecialisation::Pacifier:
		newTree->AddNode(UMaxHealthSkill::CreateMaxHealthSkill());
		newTree->AddNode(UResistanceBoostSkill::CreateResistanceBoostSkill());
		newTree->AddNode(UHealingReceivedSkill::CreateHealingRecievedSkill());
		newTree->AddNode(UShotgunDamageSkill::CreateShotgunDamageSkill());
		newTree->AddNode(UReflectDamageSkill::CreateReflectDamageSkill());
		break;

	case ESpecialisation::Havoc:
		newTree->AddNode(UAttackSpeedSkill::CreateAttackSpeedSkill());
		newTree->AddNode(UCriticalStrikeSkill::CreateCriticalStrikeSkill());
		break;

	case ESpecialisation::Invigorator:
		break;

	case ESpecialisation::Defiler:
		break;
	default:
		break;
	}

	return newTree;
}

void USkillTree::AddNode(USkillTreeNode* newNode)
{
	nodes.Add(newNode);
}

void USkillTree::RemoveNode(USkillTreeNode* nodeToRemove)
{
	nodes.Remove(nodeToRemove);
}

void USkillTree::PreHealthChanged(FHealthChange& healthChange)
{
	for (USkillTreeNode* node : GetNodes())
	{
		node->PreHealthChanged(healthChange);
	}
}
