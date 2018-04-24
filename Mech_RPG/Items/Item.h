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
		Armour,
		Weapon,
		Resource,
		End
	};
}

UENUM(BlueprintType)
namespace QualityEnums
{
	enum Quality
	{
		Base,
		Iron,
		Steel,
		Titianium,
		Unobtainium,
		End
	};
}

#pragma once
#include "UnrealString.h"
#include "Object.h"
#include "Item.generated.h"

class AMech_RPGCharacter;

UCLASS(Blueprintable)
class UItem : public UObject
{
	GENERATED_BODY()
public:
	UItem();

	UFUNCTION(BlueprintCallable, Category = "Item")
		static UItem* CreateItemByType(AMech_RPGCharacter* inOwner, ItemEnumns::ItemType type, int32 grade, int32 quality);

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
		void TakeFrom(UItem* otherItem);

	UFUNCTION(BlueprintCallable, Category = "Item")
		int32 GetGrade();

	UFUNCTION(BlueprintCallable, Category = "Item")
		void SetGrade(int32 newVal);

	UFUNCTION(BlueprintCallable, Category = "Item")
		AMech_RPGCharacter* GetItemOwner();

	UFUNCTION(BlueprintCallable, Category = "Item")
		virtual void SetItemOwner(AMech_RPGCharacter* newVal);

	UFUNCTION(BlueprintCallable, Category = "Item")
		bool HasSpace();

	UFUNCTION(BlueprintCallable, Category = "Item")
		int32 GetRemainingSpace();

	UFUNCTION(BlueprintCallable, Category = "Item")
		int32 GetAmount();

	UFUNCTION(BlueprintCallable, Category = "Item")
		void SetAmount(int32 newVal);

	UFUNCTION(BlueprintCallable, Category = "Item")
		int32 GetStackSize();

	UFUNCTION(BlueprintCallable, Category = "Item")
		void SetStackSize(int32 newVal);

	UFUNCTION(BlueprintCallable, Category = "Item")
		virtual UItem* Copy();

	UFUNCTION(BlueprintCallable, Category = "Item")
		virtual FString GetTooltipText();

	UFUNCTION(BlueprintCallable, Category = "Item")
		static UItem* CreateItem(AMech_RPGCharacter* inOwner, FString inName = "Test", int32 inAmount = 1, int32 inGrade = 0, int32 inQuality = 0, int32 inStackSize = 3);

	FORCEINLINE UItem& operator+(UItem &aitem)
	{
		UItem* nitem = aitem.Copy();
		nitem->amount = aitem.amount + amount;
		return *nitem;
	}

	FORCEINLINE UItem& operator+=(UItem &aitem)
	{
		amount += aitem.amount;
		return *this;
	}

	FORCEINLINE UItem& operator-(UItem &aitem)
	{
		UItem* nitem = aitem.Copy();
		nitem->amount = aitem.amount - amount;
		return *nitem;
	}

	FORCEINLINE UItem& operator-=(UItem &aitem)
	{
		amount -= aitem.amount;
		return *this;
	}

	static int32 HighestItemLevel;

	void CloneItemSettings(UItem* cloneFromItem);
protected:
	UPROPERTY()
		TEnumAsByte<ItemEnumns::ItemType> type;

	UPROPERTY()
		FString name;

	UPROPERTY()
		int32 grade = 1;

	UPROPERTY()
		AMech_RPGCharacter* itemOwner;

	UPROPERTY()
		int32 amount = 1;

	UPROPERTY()
		int32 stackSize = 1;

	UPROPERTY()
		int32 quality = 0;
};