// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "SkillTree/SKillTree.h"
#include "SkillTreeUI.h"

int USkillTreeUI::GetLevel()
{
	return GetSkillTree()->GetLevel();
}

ESpecialisation USkillTreeUI::GetSpec()
{
	return GetSkillTree()->GetSpec();
}

TArray<USkillTreeNode *>& USkillTreeUI::GetNodes()
{
	return GetSkillTree()->GetNodes();
}
