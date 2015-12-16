// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "MiscLibrary.h"
#include "Mech_RPGPlayerController.h"

UGroup* UMiscLibrary::playerGroup = NULL;

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
	return character != NULL && &character != NULL && !character->IsDead();
}

void UMiscLibrary::OpenCharacterPane(UWorld* world) {
	AMech_RPGCharacter* character = Cast<AMech_RPGCharacter>(UGameplayStatics::GetPlayerCharacter(world, 0));

	if (character != NULL && character->GetController() != NULL) {
		Cast<AMech_RPGPlayerController>(character->GetController())->OpenCharacterPane();
	}
}

UGroup* UMiscLibrary::GetPlayerGroup() {
	//if (playerGroup == NULL || playerGroup->GetPlayer() == NULL) {
		for (AMech_RPGCharacter* character : AMech_RPGCharacter::GetCharacters()) {
			if (IsCharacterAlive(character)
				&& IsValid(character)
				&& character->IsValidLowLevel()
				&& character->GetController() != NULL
				&& character->GetController()->GetClass()->IsChildOf(AMech_RPGPlayerController::StaticClass())) {
				return character->GetGroup();
			}
		}
	//}
	return NULL;
}

AMech_RPGCharacter* UMiscLibrary::GetPlayer() {
	//return playerGroup != NULL ? playerGroup->GetPlayer() : GetPlayerGroup() != NULL ? playerGroup->GetPlayer() : NULL;
	for (AMech_RPGCharacter* character : AMech_RPGCharacter::GetCharacters()) {
		if (IsCharacterAlive(character)
			&& IsValid(character)
			&& character->IsValidLowLevel()
			&& character->GetController() != NULL
			&& character->GetController()->GetClass()->IsChildOf(AMech_RPGPlayerController::StaticClass())) {
			return character;
		}
	}
	return NULL;
}

float UMiscLibrary::GetWidgetYaw(UCameraComponent* camera) {

	if (camera != NULL) {
		return camera->GetComponentRotation().Yaw + 90;
	}
	return 0;
}

UWorld* UMiscLibrary::GetActorWorld(AActor* actor) {
	return actor != NULL ? actor->GetWorld() : NULL;
}

TArray<AMech_RPGCharacter*> UMiscLibrary::GetCharactersInRange(float range, AActor* origin) {
	TArray<AMech_RPGCharacter*> characters;
	for (AMech_RPGCharacter* character : AMech_RPGCharacter::GetCharacters()) {
		if (IsCharacterAlive(character)
			&& IsValid(origin)
			&& origin->IsValidLowLevel()
			&& IsValid(character)
			&& character->IsValidLowLevel()) {

			if (character->GetDistanceTo(origin) <= range)
				characters.Add(character);
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

template<class T>
T* UMiscLibrary::SpawnCharacter(UWorld* world, FVector location, FRotator rotation, TSubclassOf<class AMech_RPGCharacter> classToSpawn) {
	int count = 0;
	FNavLocation nav;
	AMech_RPGCharacter* character = NULL;

	while (character == NULL && count < 10) {
		world->GetNavigationSystem()->GetRandomPointInNavigableRadius(location, 100, nav);
		character = world->SpawnActor<T>(classToSpawn, nav.Location, rotation);
		count++;
	}

	if (character != NULL) {
		character->SpawnDefaultController();
		return dynamic_cast<T*>(character);
	}
	else return NULL;
}


int UMiscLibrary::GetRandomEnum(int end) {
	return rand() % (end - 1);
}