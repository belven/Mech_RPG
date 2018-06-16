// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "SkillTree.h"




float USkillTree::GetStatBonus(EStatEnum statType)
{

	return 0;
}

USkillTree* USkillTree::CreateSkillTree(AMech_RPGCharacter * newOwner, ESpecialisation specialisation)
{
	USkillTree* newTree = NewObject<USkillTree>(StaticClass());

	switch (specialisation)
	{
	case ESpecialisation::Pacifier:
		break;

	case ESpecialisation::Havoc:
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
