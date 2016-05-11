// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "KillQuestObjective.h"
#include "Quest.h"

void UKillQuestObjective::SetUpListeners(UQuest* inQuest) {
	Super::SetUpListeners(inQuest);
	if (inQuest != nullptr && inQuest->GetOwner() != nullptr) {
		AMech_RPGCharacter* character = inQuest->GetOwner();
		character->GetGroup()->OnGroupEnemyKilled.AddUniqueDynamic(this, &UKillQuestObjective::EnemyKilled);
	}
}

bool UKillQuestObjective::IsComplete()
{
	return kills == killCount;
}

FString UKillQuestObjective::GetObjectiveText() {
	return "Kills " + FString::FromInt(kills) + "/" + FString::FromInt(killCount);
}

void UKillQuestObjective::EnemyKilled(AMech_RPGCharacter* character)
{
	if (character->GetClass()->IsChildOf(killClass) && !IsComplete()) {
		kills += 1;
		NotifyQuest();
	}
	else if (IsComplete()) {
		character->GetGroup()->OnGroupEnemyKilled.RemoveDynamic(this, &UKillQuestObjective::EnemyKilled);
	}
}

UKillQuestObjective* UKillQuestObjective::CreateKillQuestObjective(FString name, int32 inKillCount, TSubclassOf<AMech_RPGCharacter> inKillClass)
{
	UKillQuestObjective* Objective = NewObject<UKillQuestObjective>(UKillQuestObjective::StaticClass());
	Objective->killCount = inKillCount;
	Objective->killClass = inKillClass;
	Objective->SetObjectiveName(name);
	return Objective;
}