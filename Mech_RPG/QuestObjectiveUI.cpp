// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "QuestObjectiveUI.h"
#include "Quests/QuestObjective.h"


UQuestObjective* UQuestObjectiveUI::GetObjective()
{
	return objective;
}

void UQuestObjectiveUI::SetObjective(UQuestObjective* inObjective)
{
	objective = inObjective;
}

FString UQuestObjectiveUI::GetObjectiveName()
{
	return objective->GetObjectiveName();
}

FString UQuestObjectiveUI::GetObjectiveText()
{
	return objective->GetObjectiveText();

}
