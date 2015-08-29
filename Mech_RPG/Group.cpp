// Fill out your copyright notice in the Description page of Project Settings.

#include "Mech_RPG.h"
#include "Group.h"
#include "Mech_RPGCharacter.h"

UGroup* UGroup::CreateGroup(int32 inID, TArray<AMech_RPGCharacter*> inMemebrs){
	UGroup* newGroup = NewObject<UGroup>(UGroup::StaticClass());
	newGroup->SetID(inID);
	newGroup->SetMembers(inMemebrs);
	return  newGroup;
}

int32 UGroup::GetID(){
	return id;
}

TArray<AMech_RPGCharacter*> UGroup::GetMembers(){
	return members;
}

void UGroup::SetID(int32 newVal){
	id = newVal;
}

void UGroup::SetMembers(TArray<AMech_RPGCharacter*> newVal){
	members = newVal;
}

void UGroup::AddMemeber(AMech_RPGCharacter* memberToAdd){
	members.Add(memberToAdd);
}

void UGroup::RemoveMember(AMech_RPGCharacter* memberToRemove){
	members.Remove(memberToRemove);
}

bool UGroup::Compare(UGroup* inGroup){
	return inGroup->GetID() == this->GetID();
}