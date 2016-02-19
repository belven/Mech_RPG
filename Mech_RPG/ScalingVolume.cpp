// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "ScalingVolume.h"

void AScalingVolume::BeginPlay() {
	if (classToSpawn != NULL) {
		AActor* actor = GetWorld()->SpawnActor<AActor>(classToSpawn, GetActorLocation(), GetActorRotation());
		actor->SetActorTransform(GetTransform());
	}
}