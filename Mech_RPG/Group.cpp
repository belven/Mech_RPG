// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "Mech_RPG.h"
#include "Group.h"
#include "Characters/Mech_RPGCharacter.h"
#include "Mech_RPGPlayerController.h"
#include "Items/Item.h"
#include "Interactable.h"
#include "Quests/Quest.h"

UGroup::UGroup() : Super()
{

}

UGroup* UGroup::CreateGroup(int32 inID)
{
	UGroup* newGroup = NewObject<UGroup>(UGroup::StaticClass());
	newGroup->SetID(inID);
	return newGroup;
}

int32 UGroup::GetID()
{
	return id;
}

TArray<AMech_RPGCharacter*>& UGroup::GetMembers()
{
	return members;
}

void UGroup::SetID(int32 newVal)
{
	id = newVal;
}

void UGroup::SetMembers(TArray<AMech_RPGCharacter*> newVal)
{
	members = newVal;
}

void UGroup::AddMemeber(AMech_RPGCharacter* memberToAdd)
{
	members.Add(memberToAdd);
}

void UGroup::RemoveMember(AMech_RPGCharacter* memberToRemove)
{
	members.Remove(memberToRemove);
}

bool UGroup::Compare(UGroup* inGroup)
{
	return inGroup && this ? inGroup->GetID() == this->GetID() : true;
}

bool UGroup::IsGroupdInCombat()
{
	for (AMech_RPGCharacter* member : members)
	{
		if (UMiscLibrary::IsCharacterAlive(member) && member->IsInCombat())
		{
			return true;
		}
	}
	return false;
}

AMech_RPGCharacter* UGroup::GetMember(int32 index)
{
	return members.IsValidIndex(index - 1) ? members[index - 1] : nullptr;
}

AMech_RPGCharacter* UGroup::GetRandomMember()
{
	TArray<AMech_RPGCharacter*> aliveMembers;

	for (AMech_RPGCharacter* member : members)
	{
		if (UMiscLibrary::IsCharacterAlive(member))
		{
			aliveMembers.Add(member);
		}
	}

	if (aliveMembers.Num() == 0)
	{
		return nullptr;
	}
	else if (aliveMembers.Num() == 1)
	{
		return aliveMembers[0];
	}
	else
	{
		return aliveMembers[rand() % (aliveMembers.Num() - 1)];
	}
}

AMech_RPGCharacter* UGroup::GetLowHealthMember()
{
	AMech_RPGCharacter* lowestHealthMember = nullptr;
	for (AMech_RPGCharacter* member : members)
	{
		if (UMiscLibrary::IsCharacterAlive(member) && UMiscLibrary::GetMissingHealth(member) > 1)
		{
			if (lowestHealthMember != nullptr && UMiscLibrary::GetMissingHealth(lowestHealthMember) < UMiscLibrary::GetMissingHealth(member))
			{
				lowestHealthMember = member;
			}
			else
			{
				lowestHealthMember = member;
			}
		}
	}
	return lowestHealthMember;
}

AMech_RPGCharacter* UGroup::GetPlayer()
{
	if (HasMemebers())
	{
		for (AMech_RPGCharacter* character : members)
		{
			if (character != nullptr
				&& character->GetController() != nullptr
				&& mIsChildOf(character->GetController(), AMech_RPGPlayerController::StaticClass()))
			{
				return character;
			}
		}
	}

	return nullptr;
}


void UGroup::GroupMemberHit(AMech_RPGCharacter* attacker, AMech_RPGCharacter* damagedMember)
{
	if (OnMemberDamageEvent.IsBound() && attacker != nullptr && !attacker->IsDead())
	{
		OnMemberDamageEvent.Broadcast(attacker, damagedMember);
	}
}

bool UGroup::HasMemebers()
{
	return members.Num() > 0;
}

void UGroup::GroupEnemyKilled(AMech_RPGCharacter* character)
{
	if (OnGroupEnemyKilled.IsBound()) OnGroupEnemyKilled.Broadcast(character);
}

void UGroup::GroupMemberKilled(AMech_RPGCharacter* character)
{
	if (OnGroupMemberKilled.IsBound()) OnGroupMemberKilled.Broadcast(character);
}

TEnumAsByte<GroupEnums::Role> UGroup::GetRandomRole()
{
	return (GroupEnums::Role)(UMiscLibrary::GetRandomEnum(GroupEnums::End));
}

void UGroup::Interact(AInteractable * interactable)
{
	if (OnInteractEvent.IsBound())
	{
		OnInteractEvent.Broadcast(interactable);
	}
}

void UGroup::ItemPickup(AItem* item)
{
	if (OnItemPickUpEvent.IsBound())
	{
		OnItemPickUpEvent.Broadcast(item);
	}
}

void UGroup::NPCInteract(AMech_RPGCharacter * character)
{
	if (OnNPCInteractEvent.IsBound())
	{
		OnNPCInteractEvent.Broadcast(character);
	}
}

void UGroup::AddQuest(UQuest* newQuest)
{
	quests.Add(newQuest);
}

void UGroup::RemoveQuest(UQuest* questToRemove)
{
	quests.Remove(questToRemove);
}

TArray<UQuest*>& UGroup::GetQuests()
{
	return quests;
}

TArray<UQuest*> UGroup::GetCompletedQuests()
{
	TArray<UQuest*> completedQuests;

	for (UQuest* quest : quests)
	{
		if (quest->IsComplete())
		{
			completedQuests.Add(quest);
		}
	}

	return completedQuests;
}

TArray<UQuest*> UGroup::GetActiveQuests()
{
	TArray<UQuest*> activeQuests;

	for (UQuest* quest : quests)
	{
		if (!quest->IsComplete())
		{
			activeQuests.Add(quest);
		}
	}

	return activeQuests;
}