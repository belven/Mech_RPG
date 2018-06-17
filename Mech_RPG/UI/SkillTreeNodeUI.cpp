// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "SkillTree/SkillTreeNode.h"
#include "SkillTreeNodeUI.h"

int USkillTreeNodeUI::GetId()
{
	return owningNode->GetId();
}

FString USkillTreeNodeUI::GetName()
{
	return owningNode->GetName();
}

int USkillTreeNodeUI::GetStartLevel()
{
	return owningNode->GetStartLevel();
}

int USkillTreeNodeUI::GetCurrentLevel()
{
	return owningNode->GetCurrentLevel();
}

void USkillTreeNodeUI::SetCurrentLevel(int val)
{
	return owningNode->SetCurrentLevel(val);
}

int USkillTreeNodeUI::GetMaxLevel()
{
	return owningNode->GetMaxLevel();
}

FString USkillTreeNodeUI::GetTooltipText()
{
	return owningNode->GetTooltipText();
}
