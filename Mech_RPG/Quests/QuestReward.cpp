// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "QuestReward.h"
#include "Quest.h"
#include "Characters/Mech_RPGCharacter.h"

void UQuestReward::GiveReward(AMech_RPGCharacter* questOwner) {
	if (questToGive != nullptr) {
		questOwner->AddQuest(questToGive);
	}
}

UQuestReward* UQuestReward::CreateQuestReward() {
	UQuestReward* tempQuestReward = NewObject<UQuestReward>(UQuestReward::StaticClass());
	return tempQuestReward;
}