
UENUM(BlueprintType)
namespace GroupEnums {
	enum Role {
		None,
		DPS,
		Healer,
		Tank,
		Sniper,
		RPG
	};
}
#pragma once

#include "Object.h"
#include "Array.h"
#include "Group.generated.h"

class AMech_RPGCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FMemberDamageEvent, AMech_RPGCharacter*, attacker, AMech_RPGCharacter*, damagedMember);

UCLASS()
class MECH_RPG_API UGroup : public UObject {
	GENERATED_BODY()
private:
	int32 id;

	UPROPERTY()
		TArray<AMech_RPGCharacter*> members;
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
		AMech_RPGCharacter* GetPlayer();

	UFUNCTION(BlueprintCallable, Category = "Group")
		void GroupMemberHit(AMech_RPGCharacter* attacker, AMech_RPGCharacter* damagedMember);

		FMemberDamageEvent OnMemberDamageEvent;
};
