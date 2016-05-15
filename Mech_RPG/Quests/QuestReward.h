// Copyright of Explosive Industries

#pragma once

#include "Object.h"
#include "QuestReward.generated.h"

class AMech_RPGCharacter;

UCLASS()
class MECH_RPG_API UQuestReward : public UObject
{
	GENERATED_BODY()
private:
	UQuest* questToGive;
	UQuest* owningQuest;

public:

	UFUNCTION(BlueprintCallable, Category = Quest)
		virtual	void GiveReward(AMech_RPGCharacter* questOwner);

	UFUNCTION(BlueprintCallable, Category = Quest)
		static UQuestReward* CreateQuestReward();

	UFUNCTION(BlueprintCallable, Category = Quest)
		UQuest* GetQuestToGive() {
		return questToGive;
	}

	UFUNCTION(BlueprintCallable, Category = Quest)
		void SetQuestToGive(UQuest* val) {
		questToGive = val;
	}

	UFUNCTION(BlueprintCallable, Category = "Quest")
		UQuest* GetOwningQuest() {
		return owningQuest;
	}

	UFUNCTION(BlueprintCallable, Category = "Quest")
		void SetOwningQuest(UQuest* val) {
		owningQuest = val;
	}
};
