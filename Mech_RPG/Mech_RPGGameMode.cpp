// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "Mech_RPG.h"
#include "Mech_RPGGameMode.h"
#include "Mech_RPGPlayerController.h"
#include "Mech_RPGCharacter.h"

AMech_RPGGameMode::AMech_RPGGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AMech_RPGPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}