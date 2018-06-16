// Copyright of Explosive Industries

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Enums.h"
#include "SkillTreeNode.generated.h"

/**
 * 
 */
UCLASS()
class MECH_RPG_API USkillTreeNode : public UObject
{
	GENERATED_BODY()
	
private:
	int id;
	FString name;
	int startLevel;
	int currentLevel;
	int maxLevel;
	TMap<EStatEnum, float> EMPTY_MAP;

public:
	virtual TMap<EStatEnum, float> GetStatBonuses()
	{
		check(0 && "You must override this") return EMPTY_MAP;
	};
	
};
