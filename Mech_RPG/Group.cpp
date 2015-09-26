// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "Mech_RPG.h"
#include "Group.h"
#include "Mech_RPGCharacter.h"
#include "Mech_RPGPlayerController.h"

UGroup::UGroup() : Super() {

}

UGroup* UGroup::CreateGroup(int32 inID) {
	UGroup* newGroup = NewObject<UGroup>(UGroup::StaticClass());
	newGroup->SetID(inID);
	return newGroup;
}

int32 UGroup::GetID() {
	return id;
}

TArray<AMech_RPGCharacter*>& UGroup::GetMembers() {
	return members;
}

void UGroup::SetID(int32 newVal) {
	id = newVal;
}

void UGroup::SetMembers(TArray<AMech_RPGCharacter*> newVal) {
	members = newVal;
}

void UGroup::AddMemeber(AMech_RPGCharacter* memberToAdd) {
	members.Add(memberToAdd);
}

void UGroup::RemoveMember(AMech_RPGCharacter* memberToRemove) {
	members.Remove(memberToRemove);
}

bool UGroup::Compare(UGroup* inGroup) {
	return inGroup && this ? inGroup->GetID() == this->GetID() : true;
}

AMech_RPGCharacter* UGroup::GetMember(int32 index) {
	return members.IsValidIndex(index - 1) ? members[index - 1] : NULL;
}

AMech_RPGCharacter* UGroup::GetPlayer() {
	if (members.Num() > 0) {
		for (AMech_RPGCharacter* character : members) {
			if (character != NULL 
				&& character->GetController() != NULL 
				&& character->GetController()->GetClass()->IsChildOf(AMech_RPGPlayerController::StaticClass())) {
				return character;
			}
		}
	}

	return NULL;
}


void UGroup::GroupMemberHit(AMech_RPGCharacter* attacker, AMech_RPGCharacter* damagedMember) {
	if (OnMemberDamageEvent.IsBound() && attacker != NULL && !attacker->IsDead()) {
		OnMemberDamageEvent.Broadcast(attacker, damagedMember);
	}
}