// Copyright of Explosive Industries

#pragma once
#include "Array"
#include "GameFramework/Pawn.h"
#include "Cover.generated.h"

class AMech_RPGCharacter;

UCLASS()
class MECH_RPG_API ACover : public APawn {
	GENERATED_BODY()

private:

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cover")
		float coverMaxHealth;

	float currentHealth;

	void ChangeHealth(struct FHealthChange damage);

	// Sets default values for this pawn's properties
	ACover();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;


};
