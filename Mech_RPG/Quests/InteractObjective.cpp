// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "InteractObjective.h"
#include "Interactable.h"
#include "Mech_RPGCharacter.h"
#include "Quest.h"

void UInteractObjective::SetUpListeners(UQuest* inQuest)
{
	Super::SetUpListeners(inQuest);
	quest->GetOwner()->GetGroup()->OnInteractEvent.AddDynamic(this, &UInteractObjective::PlayerInteraction);
}

bool UInteractObjective::IsComplete()
{
	return complete;
}

FString UInteractObjective::GetObjectiveText()
{
	return IsComplete() ? "Task Complete" : "Activate " + interactable->GetName();
}

void UInteractObjective::PlayerInteraction(AInteractable* inInteractable)
{
	if (inInteractable == interactable) {
		complete = true;
		NotifyQuest();
	}
}

UInteractObjective* UInteractObjective::CreateInterractObjective(FString name, AInteractable* inInteractable)
{
	UInteractObjective* Objective = NewObject<UInteractObjective>(UInteractObjective::StaticClass());
	Objective->interactable = inInteractable;
	Objective->SetObjectiveName(name);
	return Objective;
}
