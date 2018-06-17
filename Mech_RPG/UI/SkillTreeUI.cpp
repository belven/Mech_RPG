// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "SkillTree/SKillTree.h"
#include "SkillTreeUI.h"

int USkillTreeUI::GetLevel()
{
	return skillTree->GetLevel();
}

ESpecialisation USkillTreeUI::GetSpec()
{
	return skillTree->GetSpec();
}
