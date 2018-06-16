// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "SkillTree.h"

float USkillTree::GetStatBonus(EStatEnum statType)
{
	float total = 0;
	for (USkillTreeNode* node : GetNodes()) 
	{
		total += node->GetStatBonus(statType);
	}
	return total;
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
