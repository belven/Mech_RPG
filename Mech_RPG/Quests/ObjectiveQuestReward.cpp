// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "ObjectiveQuestReward.h"
#include "QuestObjective.h"
#include "Quest.h"

void UObjectiveQuestReward::GiveReward(AMech_RPGCharacter* questOwner)
{
	GetOwningQuest()->AddObjective(GetObjective());
	Super::GiveReward(questOwner);
}

UObjectiveQuestReward* UObjectiveQuestReward::CreateObjectiveQuestReward(UQuestObjective* inObjective, UQuest* inQuestChain)
{
	UObjectiveQuestReward* reward = NewObject<UObjectiveQuestReward>(StaticClass());
	reward->SetObjective(inObjective);
	reward->SetQuestToGive(inQuestChain);
	return reward;
}
