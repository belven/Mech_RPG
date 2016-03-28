// Copyright of Explosive Industries

#pragma once

#include "Object.h"
#include "QuestReward.generated.h"

class AMech_RPGCharacter;
class AItem;

UCLASS()
class MECH_RPG_API UQuestReward : public UObject
{
	GENERATED_BODY()
private:
	TArray<AItem*> rewards;

public:

	UFUNCTION(BlueprintCallable, Category = Quest)
		void GiveReward(AMech_RPGCharacter* questOwner);

	UFUNCTION(BlueprintCallable, Category = Quest)
		static UQuestReward* CreateQuestReward();

	UFUNCTION(BlueprintCallable, Category = Quest)
		TArray<AItem*> GetRewards();

	UFUNCTION(BlueprintCallable, Category = Quest)
		void SetRewards(TArray<AItem*> newVal);
	
	
	
};
