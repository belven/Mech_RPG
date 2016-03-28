// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "KillQuestObjective.h"
#include "Quest.h"

void UKillQuestObjective::SetUpListeners(UQuest* quest) {
	Super::SetUpListeners(quest);
	if (quest->GetOwner() != NULL) {
		AMech_RPGCharacter* character = quest->GetOwner();
		character->GetGroup()->OnGroupEnemyKilled.AddUniqueDynamic(this, &UKillQuestObjective::EnemyKilled);
	}
}

bool UKillQuestObjective::IsComplete()
{
	return killCount <= 0;
}

FString UKillQuestObjective::GetObjectiveText() {
	return "";
}

void UKillQuestObjective::EnemyKilled(AMech_RPGCharacter* character)
{
	if (character->GetClass()->IsChildOf(killClass)) {
			killCount -= 1;
			NotifyQuest();
	}
}

UKillQuestObjective* UKillQuestObjective::CreateKillQuestObjective(int32 inKillCount, TSubclassOf<AMech_RPGCharacter> inKillClass)
{
	UKillQuestObjective* Objective = NewObject<UKillQuestObjective>(UKillQuestObjective::StaticClass());
	Objective->killCount = inKillCount;
	Objective->killClass = inKillClass;
	return Objective;
}