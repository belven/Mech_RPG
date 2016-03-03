///////////////////////////////////////////////////////////
//  AItem.h
//  Implementation of the Class AItem
//  Created on:      04-May-2015 11:16:11
//  Original author: sam
///////////////////////////////////////////////////////////

UENUM(BlueprintType, Blueprintable)
namespace ItemEnumns
{
	enum ItemType
	{
		Resource,
		Armour,
		Weapon
	};
}

#pragma once
#include "UnrealString.h"
#include "Item.generated.h"

class AMech_RPGCharacter;

UCLASS(Blueprintable)
class AItem : public AActor
{
	GENERATED_BODY()
public:
	AItem();


	UFUNCTION(BlueprintCallable, Category = "Item")
		ItemEnumns::ItemType GetType();

	UFUNCTION(BlueprintCallable, Category = "Item")
		void SetQuality(int32 newQuality);
	UFUNCTION(BlueprintCallable, Category = "Item")
		int32 GetQuality();

	UFUNCTION(BlueprintCallable, Category = "Item")
		void SetType(ItemEnumns::ItemType newVal);

	UFUNCTION(BlueprintCallable, Category = "Item")
		FString GetName();

	UFUNCTION(BlueprintCallable, Category = "Item")
		void SetName(FString newVal);

	UFUNCTION(BlueprintCallable, Category = "Item")
		int32 GetGrade();

	UFUNCTION(BlueprintCallable, Category = "Item")
		void SetGrade(int32 newVal);

	UFUNCTION(BlueprintCallable, Category = "Item")
		AMech_RPGCharacter* GetOwner();

	UFUNCTION(BlueprintCallable, Category = "Item")
		virtual void SetOwner(AMech_RPGCharacter* newVal);

	UFUNCTION(BlueprintCallable, Category = "Item")
		int32 GetAmount();

	UFUNCTION(BlueprintCallable, Category = "Item")
		void SetAmount(int32 newVal);

	UFUNCTION(BlueprintCallable, Category = "Item")
		int32 GetStackSize();

	UFUNCTION(BlueprintCallable, Category = "Item")
		void SetStackSize(int32 newVal);

	UFUNCTION(BlueprintCallable, Category = "Item")
		AItem* Copy();

	UFUNCTION(BlueprintCallable, Category = "Item")
		static AItem* CreateItem(UWorld* world, AMech_RPGCharacter* inOwner, FString inName = "Test", int32 inAmount = 1, int32 inGrade = 0, int32 inQuality = 0, int32 inStackSize = 1);

	FORCEINLINE AItem& operator+(AItem &aitem)
	{
		AItem* nitem = NewObject<AItem>(AItem::StaticClass());
		if (nitem)
		{
			nitem->amount = aitem.amount + amount;
		}

		return *nitem;
	}

	FORCEINLINE AItem& operator+=(AItem &aitem)
	{
		amount += aitem.amount;

		return *this;
	}
	FORCEINLINE AItem& operator-(AItem &aitem)
	{
		AItem* nitem = NewObject<AItem>(AItem::StaticClass());
		if (nitem)
		{
			nitem->amount = aitem.amount - amount;
		}

		return *nitem;
	}

	FORCEINLINE AItem& operator-=(AItem &aitem)
	{
		amount -= aitem.amount;

		return *this;
	}

protected:
	TEnumAsByte<ItemEnumns::ItemType> type;
	FString name;
	int32 grade;
	AMech_RPGCharacter* owner;
	int32 amount;
	int32 stackSize;
	int32 quality;

};