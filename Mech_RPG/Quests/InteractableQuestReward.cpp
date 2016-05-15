// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "InteractableQuestReward.h"
#include "Interactable.h"

void UInteractableQuestReward::GiveReward(AMech_RPGCharacter* questOwner)
{
	if (interactable != nullptr) {
		interactable->Interact(questOwner);
	}

	Super::GiveReward(questOwner);
}

UInteractableQuestReward* UInteractableQuestReward::CreateInteractableQuestReward(AInteractable* inInteractable, UQuest* inQuestChain)
{
	UInteractableQuestReward* reward = NewObject<UInteractableQuestReward>(StaticClass());
	reward->SetInteractable(inInteractable);
	reward->SetQuestToGive(inQuestChain);
	return reward;
}
