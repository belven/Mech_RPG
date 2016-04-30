// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "Quest.h"
#include "QuestReward.h"
#include "QuestObjective.h"
#include "Mech_RPGCharacter.h"

/* Creates a new quest with the given parameters, the last two are optional */
UQuest* UQuest::CreateQuest(AMech_RPGCharacter* character, TArray<UQuestObjective*> objectives, TArray<UQuestReward*> rewards, FString qName) {
	UQuest* tempQuest = NewObject<UQuest>(UQuest::StaticClass());
	tempQuest->SetOwner(character);
	tempQuest->SetObjectives(objectives);
	tempQuest->SetRewards(rewards);
	tempQuest->SetQuestName(qName);
	character->AddQuest(tempQuest);

	for (UQuestObjective* objective : objectives) {
		objective->SetUpListeners(tempQuest);
	}

	return tempQuest;
}

/* This is run by objectives to inform the quest that the objective has changed state */
void UQuest::ObjectiveUpdated(UQuestObjective* objective) {
	if (objective->IsComplete() && IsComplete()) {
		GiveRewards();

		if (OnQuestCompleted.IsBound()) {
			OnQuestCompleted.Broadcast(this);
		}
	}
}

/* Loops through all objectives and returns true if all are complete */
bool UQuest::IsComplete() {
	bool complete = true;

	for (UQuestObjective* objective : GetObjectives()) {
		complete = objective->IsComplete() && complete;
	}

	return complete;
}

/* Adds a new objective to the quest */
void UQuest::AddObjective(UQuestObjective* inObjective) {
	if (inObjective != nullptr) {
		objectives.Add(inObjective);
	}
}

/* Adds a new reward to the quest */
void UQuest::AddReward(UQuestReward* inReward) {
	if (inReward != nullptr) {
		rewards.Add(inReward);
	}
}

/* Loops through the rewards giving the rewards to the quest owner */
void UQuest::GiveRewards() {
	if (GetOwner() != nullptr) {
		for (UQuestReward* reward : GetRewards()) {
			if (reward != nullptr) {
				reward->GiveReward(GetOwner());
			}
		}
	}
}

TArray<UQuestReward*>& UQuest::GetRewards() {
	return rewards;
}

void UQuest::SetRewards(TArray<UQuestReward*> newVal) {
	rewards = newVal;
}

AMech_RPGCharacter* UQuest::GetOwner() {
	return owner;
}

void UQuest::SetOwner(AMech_RPGCharacter* newVal) {
	owner = newVal;
}

TArray<UQuestObjective*>& UQuest::GetObjectives() {
	return objectives;
}

void UQuest::SetObjectives(TArray<UQuestObjective*> newVal) {
	objectives = newVal;
}

FString UQuest::GetQuestName()
{
	return questName;
}

void UQuest::SetQuestName(FString newName)
{
	questName = newName;
}

UQuestObjective* UQuest::GetFirstObjective()
{
	if (objectives.Num() > 0)
	{
		return objectives[0];
	}
	return nullptr;
}

void UQuest::AbandonQuest() {
	GetOwner()->AbandonQuest(this);
}