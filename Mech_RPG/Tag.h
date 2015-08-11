// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "UnrealString.h"
#include "Tag.generated.h"

USTRUCT(BlueprintType)
struct FTag
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere prm1, BlueprintReadWrite prm2, Category prm3 = "Name");
		FString name;

	UPROPERTY(EditAnywhere prm1, BlueprintReadWrite prm2, Category prm3 = "Tag");
		float value;
};