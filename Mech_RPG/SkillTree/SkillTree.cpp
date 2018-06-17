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
	return  round(experience / 200.0);
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

void USkillTree::OwnerChangedOthersHealth(FHealthChange& healthChange)
{
	float gainedExperience = 0;

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
	if (healthChange.weaponUsed != nullptr && mIsChildOf(healthChange.weaponUsed, UBio_Rifle::StaticClass()))
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
		&& (mIsChildOf(healthChange.weaponUsed, USniper::StaticClass())
			|| mIsChildOf(healthChange.weaponUsed, ULaserSniper::StaticClass()))
		)
	{
		return healthChange.changeAmount;
	}
	else if (healthChange.abilityUsed != nullptr &&  mIsChildOf(healthChange.abilityUsed, USnipe::StaticClass()))
	{
		return healthChange.changeAmount;
	}
	return 0;
}

float USkillTree::CheckForHavocExperience(FHealthChange &healthChange)
{
	if (healthChange.weaponUsed != nullptr && mIsChildOf(healthChange.weaponUsed, USMG::StaticClass()))
	{
		return healthChange.changeAmount;
	}
	else if (healthChange.abilityUsed != nullptr &&  mIsChildOf(healthChange.abilityUsed, UGrenade::StaticClass()))
	{
		return healthChange.changeAmount;
	}
	return 0;
}

float USkillTree::CheckForPacifierExperience(FHealthChange &healthChange)
{
	if (healthChange.weaponUsed != nullptr && mIsChildOf(healthChange.weaponUsed, UShotgun::StaticClass()))
	{
		return healthChange.changeAmount;
	}
	else if (healthChange.abilityUsed != nullptr &&  mIsChildOf(healthChange.abilityUsed, UBindLife::StaticClass()))
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
		newTree->AddNode(UDamageIncreaseSkill::CreateDamageIncreaseSkill());
		newTree->AddNode(UCriticalStrikeSkill::CreateCriticalStrikeSkill());
		break;

	case ESpecialisation::Havoc:
		newTree->AddNode(UDamageIncreaseSkill::CreateDamageIncreaseSkill());
		newTree->AddNode(UCriticalStrikeSkill::CreateCriticalStrikeSkill());
		break;

	case ESpecialisation::Invigorator:
		newTree->AddNode(UDamageIncreaseSkill::CreateDamageIncreaseSkill());
		newTree->AddNode(UCriticalStrikeSkill::CreateCriticalStrikeSkill());
		break;

	case ESpecialisation::Defiler:
		newTree->AddNode(UDamageIncreaseSkill::CreateDamageIncreaseSkill());
		newTree->AddNode(UCriticalStrikeSkill::CreateCriticalStrikeSkill());
		break;
	default:
		newTree->AddNode(UDamageIncreaseSkill::CreateDamageIncreaseSkill());
		newTree->AddNode(UCriticalStrikeSkill::CreateCriticalStrikeSkill());
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
