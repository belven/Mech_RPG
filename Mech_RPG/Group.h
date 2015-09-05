
UENUM(BlueprintType)
namespace GroupEnums {
	enum Role {
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

UCLASS()
class MECH_RPG_API UGroup : public UObject
{
	GENERATED_BODY()
private:
	int32 id;

	UPROPERTY()
	TArray<AMech_RPGCharacter*> members;

public:
	UGroup();
	UFUNCTION(BlueprintCallable, Category = "Group")
	static UGroup* CreateGroup(int32 inID);

	TArray<AMech_RPGCharacter*> GetMembers();
	void SetMembers(TArray<AMech_RPGCharacter*> newVal);

	void SetID(int32 newVal);
	int32 GetID();

	void AddMemeber(AMech_RPGCharacter* memberToAdd);
	void RemoveMember(AMech_RPGCharacter* memberToRemove);
	bool Compare(UGroup* inGroup);

	AMech_RPGCharacter* GetMember(int index);
	AMech_RPGCharacter* GetPlayer();
};
