// Copyright of Explosive Industries

#include "Mech_RPG.h"
#include "MiscLibrary.h"
#include "Mech_RPGPlayerController.h"
#include "Mech_RPGCharacter.h"
#include "Group.h"


UGroup* UMiscLibrary::playerGroup = nullptr;
AMech_RPGPlayerController* UMiscLibrary::playerController = nullptr;
TEnumAsByte<GameEnums::Difficulty> UMiscLibrary::difficulty = GameEnums::Hard;
FRotator UMiscLibrary::cameraRot = FRotator(90, 90, 0);

float UMiscLibrary::GetMissingHealth(AMech_RPGCharacter* character) {
	return character != nullptr ? character->GetMaxHealth() - character->GetHealth() : 0.0;
}

TEnumAsByte<GameEnums::Difficulty> UMiscLibrary::GetDifficulty()
{
	return difficulty;
}

AMech_RPGPlayerController* UMiscLibrary::GetPlayerController() {
	return playerController;
}

void UMiscLibrary::SetPlayerController(AMech_RPGPlayerController* newController) {
	playerController = newController;
}

template<class T>
void UMiscLibrary::InitialiseArray(TArray<T>& emptyArray, T* items) {
	emptyArray.Append(items, ARRAY_COUNT(items));
}

FColor UMiscLibrary::GetRelativeColour(AMech_RPGCharacter* character) {
	if (GetPlayer() != nullptr) {
		if (character->CompareGroup(GetPlayer())) {
			return FColor::Green;
		}
		else {
			return FColor::Red;
		}
	}
	return FColor::Red;
}

void UMiscLibrary::SetDifficulty(GameEnums::Difficulty newDifficulty)
{
	difficulty = newDifficulty;
}

bool UMiscLibrary::CanSee(UWorld* world, FVector pointA, FVector pointB) {
	TArray<FHitResult> results;
	world->LineTraceMultiByObjectType(results, pointA, pointB, ECollisionChannel::ECC_WorldStatic);
	return results.Num() == 0;
}

float UMiscLibrary::GetHealthPercent(AMech_RPGCharacter* character) {
	return character != nullptr ? character->GetHealth() / character->GetMaxHealth() : 0.0;
}

float UMiscLibrary::GetMissingHealthPercent(AMech_RPGCharacter* character) {
	return character != nullptr ? character->GetMaxHealth() / GetMissingHealth(character) : 0.0;
}

float UMiscLibrary::GetAbilityCooldownPercent(UAbility* ability) {
	return ability != nullptr ? 1 - (ability->GetCurrentTimeRemaining() / ability->GetCooldown()) : 0.0;
}

float UMiscLibrary::GetAbilityChannelPercent(UChannelledAbility* ability) {
	return ability != nullptr ? 1 - (ability->GetCurrentChannelTime() / ability->GetChannelDuration()) : 0.0;
}

bool UMiscLibrary::IsCharacterAlive(AMech_RPGCharacter* character) {
	return character != nullptr && &character != nullptr && !character->IsDead();
}

void UMiscLibrary::OpenCharacterPane(UWorld* world) {
	AMech_RPGCharacter* character = Cast<AMech_RPGCharacter>(UGameplayStatics::GetPlayerCharacter(world, 0));

	if (character != nullptr && character->GetController() != nullptr) {
		Cast<AMech_RPGPlayerController>(character->GetController())->OpenCharacterPane();
	}
}

UGroup* UMiscLibrary::GetPlayerGroup() {
	//if (playerGroup == nullptr || playerGroup->GetPlayer() == nullptr) {
	for (AMech_RPGCharacter* character : AMech_RPGCharacter::GetCharacters()) {
		if (IsCharacterAlive(character)
			&& IsValid(character)
			&& character->IsValidLowLevel()
			&& mIsChildOf(character->GetController(), AMech_RPGPlayerController::StaticClass())) {
			return character->GetGroup();
		}
	}
	//}
	return nullptr;
}

AMech_RPGCharacter* UMiscLibrary::GetPlayer() {
	if (GetPlayerController() == nullptr) {
		for (AMech_RPGCharacter* character : AMech_RPGCharacter::GetCharacters()) {
			if (IsCharacterAlive(character)
				&& IsValid(character)
				&& character->IsValidLowLevel()
				&& mIsChildOf(character->GetController(), AMech_RPGPlayerController::StaticClass())) {
				playerController = Cast<AMech_RPGPlayerController>(character->GetController());
				return character;
			}
		}
	}
	else {
		return GetPlayerController()->GetOwner();
	}
	return nullptr;
}

float UMiscLibrary::GetWidgetYaw(UCameraComponent* camera) {

	if (camera != nullptr) {
		return camera->GetComponentRotation().Yaw + 90;
	}
	return 0;
}

UWorld* UMiscLibrary::GetActorWorld(AActor* actor) {
	return actor != nullptr ? actor->GetWorld() : nullptr;
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

TArray<AMech_RPGCharacter*> UMiscLibrary::GetCharactersInRange(float range, FVector location) {
	TArray<AMech_RPGCharacter*> characters;
	for (AMech_RPGCharacter* character : AMech_RPGCharacter::GetCharacters()) {
		if (IsCharacterAlive(character)
			&& IsValid(character)
			&& character->IsValidLowLevel()) {

			if ((character->GetActorLocation() - location).Size() <= range)
				characters.Add(character);
		}
	}
	return characters;
}

bool UMiscLibrary::IsCover(AActor* character) {
	return mIsChildOf(character, ACover::StaticClass());
}

bool UMiscLibrary::IsMechCharacter(AActor* character) {
	return mIsChildOf(character, AMech_RPGCharacter::StaticClass());
}

template<class T>
T* UMiscLibrary::SpawnCharacter(UWorld* world, FVector location, FRotator rotation, TSubclassOf<class AMech_RPGCharacter> classToSpawn) {
	int count = 0;
	FNavLocation nav;
	AMech_RPGCharacter* character = nullptr;

	while (character == nullptr && count < 10) {
		FActorSpawnParameters params;
		params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
		world->GetNavigationSystem()->GetRandomPointInNavigableRadius(location, 100, nav);
		character = world->SpawnActor<T>(classToSpawn, nav.Location, rotation, params);
		count++;
	}

	if (character != nullptr) {
		character->SpawnDefaultController();
		return dynamic_cast<T*>(character);
	}
	else return nullptr;
}


bool UMiscLibrary::IsChildOf(UObject* object, UClass* inClass)
{
	return object != nullptr ? object->GetClass()->IsChildOf(inClass) : false;
}

int UMiscLibrary::GetRandomEnum(int end) {
	return rand() % (end - 1);
}