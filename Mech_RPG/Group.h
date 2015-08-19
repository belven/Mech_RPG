// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "Array.h"
#include "Group.generated.h"

class AMech_RPGCharacter;

UCLASS()
class MECH_RPG_API UGroup : public UObject
{
	GENERATED_BODY()
private:
	int32 id;
	TArray<AMech_RPGCharacter*> members;

public:
	UFUNCTION(BlueprintCallable, Category = "Group")
	static UGroup* CreateGroup(int32 inID, TArray<AMech_RPGCharacter*> inMemebrs);

	TArray<AMech_RPGCharacter*> GetMembers();
	void SetMembers(TArray<AMech_RPGCharacter*> newVal);

	void SetID(int32 newVal);
	int32 GetID();

	void AddMemeber(AMech_RPGCharacter* memberToAdd);
	void RemoveMember(AMech_RPGCharacter* memberToRemove);
	bool Compare(UGroup* inGroup);
};
