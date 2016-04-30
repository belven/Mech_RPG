// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "InteractObjective.h"
#include "Interactable.h"
#include "Mech_RPGCharacter.h"
#include "Quest.h"

void UInteractObjective::SetUpListeners(UQuest* quest)
{
	quest->GetOwner()->GetGroup()->OnInteractEvent.AddDynamic(this, &UInteractObjective::PlayerInteraction);
}

bool UInteractObjective::IsComplete()
{
	return complete;
}

FString UInteractObjective::GetObjectiveText()
{
	return "";
}

void UInteractObjective::PlayerInteraction(AInteractable * inInteractable)
{
	if (inInteractable == interactable) {
		complete = true;
		NotifyQuest();
	}
}

UInteractObjective* UInteractObjective::CreateInterractObjective(AInteractable* inInteractable)
{
	UInteractObjective* Objective = NewObject<UInteractObjective>(UInteractObjective::StaticClass());
	Objective->interactable = inInteractable;
	return Objective;
}
