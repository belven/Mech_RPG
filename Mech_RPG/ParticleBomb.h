// Copyright of Explosive Industries

#pragma once

#include "Ability.h"
#include "ParticleBomb.generated.h"

/**
 * 
 */
UCLASS()
class MECH_RPG_API UParticleBomb : public UAbility
{
	GENERATED_BODY()
	
private:
	float damage;

public:

	UFUNCTION(BlueprintCallable, Category = "Ability")
		bool Activate(class AMech_RPGCharacter* target, FVector targetLocation = FVector::ZeroVector) override;

	UFUNCTION(BlueprintCallable, Category = "Ability")
		static UParticleBomb* CreateAbility(float cooldown, AMech_RPGCharacter* owner, float inDamage);
	
	
};
