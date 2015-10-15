// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "MiscLibrary.h"


float UMiscLibrary::GetMissingHealth(AMech_RPGCharacter* character) {
	return character != NULL ? character->GetMaxHealth() - character->GetHealth() : 0.0;
}

float UMiscLibrary::GetHealthPercent(AMech_RPGCharacter* character) {
	return character != NULL ? character->GetHealth() / character->GetMaxHealth() : 0.0;
}

float UMiscLibrary::GetMissingHealthPercent(AMech_RPGCharacter* character) {
	return character != NULL ? character->GetMaxHealth() / GetMissingHealth(character) : 0.0;
}

float UMiscLibrary::GetAbilityCooldownPercent(UAbility* ability) {
	return ability != NULL ? 1 - (ability->GetCurrentTimeRemaining() / ability->GetCooldown()) : 0.0;
}

float UMiscLibrary::GetAbilityChannelPercent(UChannelledAbility* ability) {
	return ability != NULL ? 1 - (ability->GetCurrentChannelTime() / ability->GetChannelDuration()) : 0.0;
}

bool UMiscLibrary::IsCharacterAlive(AMech_RPGCharacter* character) {
	return character != NULL && !character->IsDead();
}

TArray<AMech_RPGCharacter*> UMiscLibrary::GetCharactersInRange(float range,  AActor* origin) {
	TArray<AMech_RPGCharacter*> characters;
	for (FConstPawnIterator iter = origin->GetWorld()->GetPawnIterator(); iter; iter++) {
		APawn* pawn = iter->Get();
		if (pawn != NULL && IsMechCharacter(pawn) && pawn->GetDistanceTo(origin) <= range) {
			AMech_RPGCharacter* character = Cast<AMech_RPGCharacter>(pawn);

			if (IsCharacterAlive(character)) {
				characters.Add(character);
			}
		}
	}
	return characters;
}

bool UMiscLibrary::IsCover(AActor* character) {
	return character->GetClass()->IsChildOf(ACover::StaticClass());
}

bool UMiscLibrary::IsMechCharacter(AActor* character) {
	return character->GetClass()->IsChildOf(AMech_RPGCharacter::StaticClass());
}

AMech_RPGCharacter* UMiscLibrary::SpawnCharacter(UWorld* world, FVector location, FRotator rotation, TSubclassOf<class AMech_RPGCharacter> classToSpawn) {
	AMech_RPGCharacter* character = world->SpawnActor<AMech_RPGCharacter>(classToSpawn, location, rotation);
	character->SpawnDefaultController();
	return character;
}