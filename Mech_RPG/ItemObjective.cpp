// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "ItemObjective.h"
#include "Inventory.h"
#include "Quest.h"


void UItemObjective::SetUpListeners(UQuest* quest)
{
	quest->GetOwner()->GetGroup()->OnItemPickUpEvent.AddDynamic(this, &UItemObjective::PlayerItemPickup);
}

bool UItemObjective::IsComplete()
{
	return GetQuest()->GetOwner()->GetInventory()->GetItemAmount(item->GetName()) >= item->GetAmount();
}

FString UItemObjective::GetObjectiveText()
{
	return "";
}

void UItemObjective::PlayerItemPickup(AItem* inItem)
{
	if (IsComplete()) {
		NotifyQuest();
	}
}

UItemObjective* UItemObjective::CreateItemObjective(AItem* inItem)
{
	UItemObjective* Objective = NewObject<UItemObjective>(UItemObjective::StaticClass());
	Objective->item = inItem;
	return Objective;
}