
UENUM(BlueprintType)
namespace GroupEnums {
	enum Role {
		None,
		DPS,
		Healer,
		Tank,
		Sniper,
		RPG,
		Support,
		End
	};
}
#pragma once

#include "Object.h"
#include "Array.h"
#include "Group.generated.h"

class AMech_RPGCharacter;
class AItem;
class AInteractable;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FMemberDamageEvent, AMech_RPGCharacter*, attacker, AMech_RPGCharacter*, damagedMember);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGroupEnemyKilled, AMech_RPGCharacter*, character);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGroupMemberKilled, AMech_RPGCharacter*, character);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGroupNPCInteractEvent, AMech_RPGCharacter*, character);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGroupItemPickUpEvent, AItem*, item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGroupInteractEvent, AInteractable*, interactable);

UCLASS(Blueprintable)
class MECH_RPG_API UGroup : public UObject {
	GENERATED_BODY()
private:
	int32 id;

	UPROPERTY()
		TArray<AMech_RPGCharacter*> members;

	TArray<UQuest*> quests;
public:
	UGroup();
	UFUNCTION(BlueprintCallable, Category = "Group")
		static UGroup* CreateGroup(int32 inID);

	UFUNCTION(BlueprintCallable, Category = "Group")
		TArray<AMech_RPGCharacter*>& GetMembers();

	UFUNCTION(BlueprintCallable, Category = "Group")
		void SetMembers(TArray<AMech_RPGCharacter*> newVal);

	UFUNCTION(BlueprintCallable, Category = "Group")
		void SetID(int32 newVal);

	UFUNCTION(BlueprintCallable, Category = "Group")
		int32 GetID();

	UFUNCTION(BlueprintCallable, Category = "Group")
		void AddMemeber(AMech_RPGCharacter* memberToAdd);

	UFUNCTION(BlueprintCallable, Category = "Group")
		void RemoveMember(AMech_RPGCharacter* memberToRemove);

	UFUNCTION(BlueprintCallable, Category = "Group")
		bool Compare(UGroup* inGroup);

	UFUNCTION(BlueprintCallable, Category = "Group")
		AMech_RPGCharacter* GetMember(int32 index);

	UFUNCTION(BlueprintCallable, Category = "Group")
		AMech_RPGCharacter* GetRandomMember();

	AMech_RPGCharacter * GetLowHealthMember();

	UFUNCTION(BlueprintCallable, Category = "Group")
		AMech_RPGCharacter* GetPlayer();

	UFUNCTION(BlueprintCallable, Category = "Group")
		void GroupMemberHit(AMech_RPGCharacter* attacker, AMech_RPGCharacter* damagedMember);

	bool HasMemebers();

	UPROPERTY(BlueprintAssignable, Category = "Events")
		FGroupEnemyKilled OnGroupEnemyKilled;

	UFUNCTION(BlueprintCallable, Category = "Events")
		void GroupEnemyKilled(AMech_RPGCharacter* character);

	UPROPERTY(BlueprintAssignable, Category = "Events")
		FGroupMemberKilled OnGroupMemberKilled;

	UFUNCTION(BlueprintCallable, Category = "Events")
		void GroupMemberKilled(AMech_RPGCharacter* character);
	
	UPROPERTY(BlueprintAssignable, Category = "Events")
		FMemberDamageEvent OnMemberDamageEvent;

	UPROPERTY(BlueprintAssignable, Category = "Events")
		FGroupNPCInteractEvent OnNPCInteractEvent;

	UPROPERTY(BlueprintAssignable, Category = "Events")
		FGroupItemPickUpEvent OnItemPickUpEvent;

	UPROPERTY(BlueprintAssignable, Category = "Events")
		FGroupInteractEvent OnInteractEvent;

	UFUNCTION(BlueprintCallable, Category = "Group")
		static TEnumAsByte<GroupEnums::Role> GetRandomRole();
	
	UFUNCTION(BlueprintCallable, Category = "Events")
		void Interact(AInteractable * interactable);

	UFUNCTION(BlueprintCallable, Category = "Events")
		void NPCInteract(AMech_RPGCharacter * character);

	UFUNCTION(BlueprintCallable, Category = "Events")
		void ItemPickup(AItem* item);

	UFUNCTION(BlueprintCallable, Category = "Quest")
		void AddQuest(UQuest* newQuest);

	UFUNCTION(BlueprintCallable, Category = "Quest")
		void RemoveQuest(UQuest* questToRemove);

	UFUNCTION(BlueprintCallable, Category = "Quest")
		TArray<UQuest*>& GetQuests();

	UFUNCTION(BlueprintCallable, Category = "Quest")
		TArray<UQuest*> GetCompletedQuests();

	UFUNCTION(BlueprintCallable, Category = "Quest")
		TArray<UQuest*> GetActiveQuests();
};
