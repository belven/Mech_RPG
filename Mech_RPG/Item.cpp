///////////////////////////////////////////////////////////
//  AItem.cpp
//  Implementation of the Class AItem
//  Created on:      04-May-2015 11:16:11
//  Original author: sam
///////////////////////////////////////////////////////////
#include "Mech_RPG.h"
#include "Item.h"
#include "Mech_RPGCharacter.h"

AItem::AItem() : Super(){
	SetActorEnableCollision(false);
}

ItemEnumns::ItemType AItem::GetType() {
	return type;
}

void AItem::SetType(ItemEnumns::ItemType newVal) {
	type = newVal;
}

FString AItem::GetName() {
	return name;
}

void AItem::SetName(FString newVal) {
	name = newVal;
}

int32 AItem::GetGrade() {
	return grade;
}

void AItem::SetGrade(int32 newVal) {
	grade = newVal;
}

AMech_RPGCharacter* AItem::GetOwner() {
	return owner;
}

int32 AItem::GetQuality()
{
	return quality;
}

void AItem::SetQuality(int32 newQuality)
{
	quality = newQuality;
}

void AItem::SetOwner(AMech_RPGCharacter* newVal) {
	owner = newVal;
}

int32 AItem::GetAmount() {
	return amount;
}

void AItem::SetAmount(int32 newVal) {
	amount = newVal;
}

int32 AItem::GetStackSize() {
	return stackSize;
}

void AItem::SetStackSize(int32 newVal) {
	stackSize = newVal;
}

AItem* AItem::Copy() {
	AItem* newItem = NewObject<AItem>(StaticClass());
	newItem->SetName(GetName());
	newItem->SetAmount(GetAmount());
	newItem->SetStackSize(GetStackSize());
	newItem->SetType(GetType());
	newItem->SetGrade(GetGrade());
	newItem->SetOwner(GetOwner());
	return  newItem;
}

AItem * AItem::CreateItem(UWorld* world, AMech_RPGCharacter* inOwner, FString inName, int32 inAmount, int32 inGrade, int32 inQuality, int32 inStackSize)
{
	FActorSpawnParameters params;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AItem* newItem = world->SpawnActor<AItem>(AItem::StaticClass(), inOwner->GetTransform(), params);
	newItem->SetName(inName);
	newItem->SetAmount(inAmount);
	newItem->SetStackSize(inStackSize);
	//newItem->SetType(GetType());
	newItem->SetGrade(inGrade);
	newItem->SetOwner(inOwner);
	newItem->SetQuality(inQuality);
	return  newItem;
}
