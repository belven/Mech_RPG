// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "MechAttachment.generated.h"

UCLASS(Blueprintable)
class MECH_RPG_API AMechAttachment : public AActor
{
	GENERATED_BODY()
private:
		AMech_RPGCharacter* owner;
		int32 grade;
		int32 quality;

public:	
	// Sets default values for this actor's properties
	AMechAttachment();

	int32 GetGrade();
	int32 GetQuality();

	void SetGrade(int32 newGrade);
	void SetQuality(int32 newQuality);

	AMech_RPGCharacter* GetOwner();
	virtual	void SetOwner(AMech_RPGCharacter* inOwner);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	virtual void SetActorHiddenInGame(bool bNewHidden) override;
protected:
	UStaticMeshComponent* meshComponent;
	TSubclassOf<UStaticMesh> meshClass;
	UStaticMesh* mesh;
	
};
