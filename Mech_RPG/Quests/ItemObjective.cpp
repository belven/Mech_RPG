// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "ItemObjective.h"
#include "Items/Inventory.h"
#include "Quest.h"


void UItemObjective::SetUpListeners(UQuest* inQuest)
{
	Super::SetUpListeners(inQuest);
	quest->GetOwner()->GetGroup()->OnItemPickUpEvent.AddDynamic(this, &UItemObjective::PlayerItemPickup);
}

bool UItemObjective::IsComplete()
{
	return GetQuest()->GetOwner()->GetInventory()->GetItemAmount(item->GetName()) >= item->GetAmount();
}

FString UItemObjective::GetObjectiveText()
{
	FString text = "You must collect ";

	if (!IsComplete()) {
		text = "You must collect ";
		text += FString::SanitizeFloat(item->GetAmount());
		text += " ";
		text += item->GetName();
	}
	else {
		text = "Task complete";
	}
	return text;
}

void UItemObjective::PlayerItemPickup(AItem* inItem)
{
	if (IsComplete()) {
		// Remove before notifying to avoid loops due to item adding from quest
		quest->GetOwner()->GetGroup()->OnItemPickUpEvent.RemoveDynamic(this, &UItemObjective::PlayerItemPickup);
		NotifyQuest();
	}
}

UItemObjective* UItemObjective::CreateItemObjective(FString name, AItem* inItem)
{
	UItemObjective* Objective = NewObject<UItemObjective>(UItemObjective::StaticClass());
	Objective->item = inItem;
	Objective->SetObjectiveName(name);
	return Objective;
}