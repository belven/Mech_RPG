// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "UnrealString.h"
#include "Tag.generated.h"

USTRUCT(BlueprintType)
struct FTag {
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tag")
		FString name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Name")
		float value;
};