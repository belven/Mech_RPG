#pragma once
#include "Mech_RPG.h"
#include "Boss.h"

void ABoss::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	//static int botsSpawned = 4;
	//if (GetHealth() < 800 && botsSpawned > 0) {
	//	FNavLocation loc;
	//	GetWorld()->GetNavigationSystem()->GetRandomReachablePointInRadius(GetActorLocation(), 400, loc);

	//	AMech_RPGCharacter* bot = GetWorld()->SpawnActor<AMech_RPGCharacter>(AMech_RPGCharacter::StaticClass(), loc.Location, GetActorRotation());
	//	bot->SetGroup(GetGroup());
	//	bot->CreatePresetRole();
	//	botsSpawned--;
	//}
}
