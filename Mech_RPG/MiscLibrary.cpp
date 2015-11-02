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
	return character != NULL && &character != NULL && !character->IsDead();
}

void UMiscLibrary::OpenCharacterPane(UWorld* world) {
	AMech_RPGCharacter* character = Cast<AMech_RPGCharacter>(UGameplayStatics::GetPlayerCharacter(world, 0));

	if (character != NULL && character->GetController() != NULL) {
		Cast<AMech_RPGPlayerController>(character->GetController())->OpenCharacterPane();
	}
}

float UMiscLibrary::GetWidgetYaw(UCameraComponent* camera) {

	if (camera != NULL) {
		return camera->GetComponentRotation().Yaw + 90;
	}
	return 0;
}

UWorld* UMiscLibrary::GetActorWorld(AActor* actor) {
	return actor->GetWorld();
}

TArray<AMech_RPGCharacter*> UMiscLibrary::GetCharactersInRange(float range, AActor* origin) {
	TArray<AMech_RPGCharacter*> characters;
	//for (FConstPawnIterator iter = origin->GetWorld()->GetPawnIterator(); iter; iter++) {
	//	APawn* pawn = iter->Get();
	//	if (pawn != NULL && IsMechCharacter(pawn) && pawn->GetDistanceTo(origin) <= range) {
	//		AMech_RPGCharacter* character = Cast<AMech_RPGCharacter>(pawn);

	//		if (IsCharacterAlive(character)) {
	//			characters.Add(character);
	//		}
	//	}
	//}

	for (AMech_RPGCharacter* character : AMech_RPGCharacter::GetCharacters()) {
		if (IsCharacterAlive(character) && IsValid(origin) && IsValid(character) && character->GetDistanceTo(origin) <= range) {
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