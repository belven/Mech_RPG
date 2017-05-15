// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "QuestDisplayUI.h"
#include "Characters/Mech_RPGCharacter.h"
#include "Quests/QuestManager.h"

AMech_RPGCharacter* UQuestDisplayUI::GetCharacter()
{
	return character;
}

void UQuestDisplayUI::SetCharacter(AMech_RPGCharacter* inCharacter)
{
	character = inCharacter;
}

TArray<UQuest*>& UQuestDisplayUI::GetQuests()
{
	return UQuestManager::GetQuests();
}
