// Copyright of Explosive Industries

#pragma once
#include "Mech_RPG.h"
#include "Interactable.h"
#include "Items/Item.h"
#include "Quest.h"
#include "Characters/Mech_RPGCharacter.h"
#include "QuestManager.h"

UQuestManager* UQuestManager::INSTANCE = nullptr;
//
//UQuestManager* UQuestManager::GetInstance() {
//	if (INSTANCE == nullptr) {
//		INSTANCE = NewObject<UQuestManager>(StaticClass());
//	}
//	return INSTANCE;
//}

//void UQuestManager::EntityKilled(FHealthChange healthChange) {
//	/*if (OnEntityKilled.IsBound()) {
//		OnEntityKilled.Broadcast(healthChange);
//	}*/
//}

void UQuestManager::ItemAquired(AItem* item) {
	/*if (OnItemPickUpEvent.IsBound()) {
		OnItemPickUpEvent.Broadcast(item);
	}*/
}

void UQuestManager::AbilityUsed(UAbility* abilityUsed) {
	/*if (OnAbilityUsedEvent.IsBound()) {
		OnAbilityUsedEvent.Broadcast(abilityUsed);
	}*/
}

void UQuestManager::PlayerInteraction(AInteractable* inInteractable) {
	/*if (OnInteractEvent.IsBound()) {
		OnInteractEvent.Broadcast(inInteractable);
	}*/
}

void UQuestManager::AddQuest(UQuest* newQuest) {
	return UMiscLibrary::GetPlayerGroup()->RemoveQuest(newQuest);
}

void UQuestManager::RemoveQuest(UQuest* questToRemove) {
	return UMiscLibrary::GetPlayerGroup()->RemoveQuest(questToRemove);
}

TArray<UQuest*>& UQuestManager::GetQuests()
{
	return UMiscLibrary::GetPlayerGroup()->GetQuests();
}

TArray<UQuest*> UQuestManager::GetCompletedQuests() {
	return UMiscLibrary::GetPlayerGroup()->GetCompletedQuests();
}

TArray<UQuest*> UQuestManager::GetActiveQuests() {
	return UMiscLibrary::GetPlayerGroup()->GetActiveQuests();
}