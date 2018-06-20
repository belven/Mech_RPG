// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "SkillTreeNode.h"

FString USkillTreeNode::GetTooltipText()
{
	return "";
}

void USkillTreeNode::OwnerPreChangedOthersHealth(FHealthChange& healthChange)
{
	 // Do Nothing
}

void USkillTreeNode::PreHealthChanged(FHealthChange& healthChange)
{
	// Do Nothing
}
