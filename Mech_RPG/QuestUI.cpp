// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "QuestUI.h"
#include "Quest.h"
#include "QuestObjective.h"


UQuest* UQuestUI::GetQuest()
{
	return quest;
}

void UQuestUI::SetQuest(UQuest* inQuest)
{
	quest = inQuest;
}

TArray<UQuestObjective*>& UQuestUI::GetObjectives()
{
	return quest->GetObjectives();
}

FString UQuestUI::GetQuestName()
{
	return quest->GetQuestName();
}
