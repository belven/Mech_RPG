// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "QuestObjective.h"
#include "Quest.h"


void UQuestObjective::SetUpListeners(UQuest* quest) {
	SetQuest(quest);
}


UQuest* UQuestObjective::GetQuest() {
	return quest;
}


void UQuestObjective::SetQuest(UQuest* newVal) {
	quest = newVal;
}


void UQuestObjective::SetQuestObjective(FString objectiveName) {
	SetObjectiveName(objectiveName);
}

void UQuestObjective::NotifyQuest() {
	if (GetQuest() != NULL) {
		GetQuest()->ObjectiveUpdated(this);
	}

	if (OnObjectiveCompleted.IsBound() && IsComplete()) {
		OnObjectiveCompleted.Broadcast(this);
	}
	else if (OnObjectiveUpdated.IsBound()) {
		OnObjectiveUpdated.Broadcast(this);
	}
}

FString UQuestObjective::GetObjectiveName()
{
	return objectiveName;

}

void UQuestObjective::SetObjectiveName(FString newName)
{
	objectiveName = newName;
}

