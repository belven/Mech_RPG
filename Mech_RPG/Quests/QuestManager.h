// Copyright of Explosive Industries

#pragma once

#include "Object.h"
#include "QuestManager.generated.h"

//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEntityKilled, FHealthChange, healthChange);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FItemPickUpEvent, AItem*, item);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInteractEvent, AInteractable*, interactable);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilityUsedEvent, UAbility*, ability);

class AMech_RPGCharacter;
class AItem;
class AInteractable;
class UQuest;

UCLASS(Blueprintable)
class UQuestManager : public UObject
{
	GENERATED_BODY()
private:

	static UQuestManager* INSTANCE;

public:
	//UFUNCTION(BlueprintCallable, Category = "Quest")
		//static UQuestManager* GetInstance();

	//UFUNCTION(BlueprintCallable, Category = "Quest")
	//	static void EntityKilled(FHealthChange healthChange);

	static void ItemAquired(AItem* item);

	static void AbilityUsed(UAbility* abilityUsed);

	void PlayerInteraction(AInteractable* inInteractable);

	UFUNCTION(BlueprintCallable, Category = "Quest")
		static void AddQuest(UQuest* newQuest);

	UFUNCTION(BlueprintCallable, Category = "Quest")
		static void RemoveQuest(UQuest* questToRemove);

	UFUNCTION(BlueprintCallable, Category = "Quest")
		static TArray<UQuest*>& GetQuests();

	UFUNCTION(BlueprintCallable, Category = "Quest")
		static TArray<UQuest*> GetCompletedQuests();

	UFUNCTION(BlueprintCallable, Category = "Quest")
		static	TArray<UQuest*> GetActiveQuests();

	//UPROPERTY(BlueprintAssignable, Category = "Events")
		//FEntityKilled OnEntityKilled;

	//UPROPERTY(BlueprintAssignable, Category = "Events")
	//	FItemPickUpEvent OnItemPickUpEvent;

	//UPROPERTY(BlueprintAssignable, Category = "Events")
	//	FInteractEvent OnInteractEvent;

	//UPROPERTY(BlueprintAssignable, Category = "Events")
		//FAbilityUsedEvent OnAbilityUsedEvent;
};
