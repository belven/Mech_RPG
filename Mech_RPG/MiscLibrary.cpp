// Copyright of Explosive Industries

#pragma once
#include "Mech_RPG.h"
#include "MiscLibrary.h"
#include "Mech_RPGPlayerController.h"
#include "Characters/Mech_RPGCharacter.h"
#include "Group.h"
#include "Abilities/Ability.h"
#include "Abilities/ChannelledAbility.h"
#include "Components/CapsuleComponent.h"
#include "Weapons.h"
#include "EngineUtils.h"


UGroup* UMiscLibrary::playerGroup = nullptr;
AMech_RPGPlayerController* UMiscLibrary::playerController = nullptr;
TEnumAsByte<GameEnums::Difficulty> UMiscLibrary::difficulty = GameEnums::Hard;
FRotator UMiscLibrary::cameraRot = FRotator(90, 90, 0);
const FString UMiscLibrary::lnBreak = "\n";

// If a health change value is at this or below, then it should do % change and not the actual value
const float UMiscLibrary::MAX_HEALTH_CHANGE = 2;

float UMiscLibrary::GetMissingHealth(AMech_RPGCharacter* character)
{
	return character != nullptr ? character->GetMaxHealth() - character->GetHealth() : 0.0;
}

class UWeapon* UMiscLibrary::CreatePresetWeapon(class AMech_RPGCharacter* inOwner, TEnumAsByte<WeaponEnums::WeaponType> weaponType, int32 weaponGrade, int32 weaponQuality)
{
	UWeapon* weapon = nullptr;

	switch (weaponType)
	{
	case WeaponEnums::SMG: {
		weapon = USMG::CreateSMG(inOwner);
		break;
	}
	case WeaponEnums::Drone_Weapon: {
		FOverheatWeaponParams overheatSettings;
		overheatSettings.healthChange = 20;
		overheatSettings.range = 1400;
		overheatSettings.fireRate = 0.3;
		overheatSettings.heals = false;
		overheatSettings.heatLosePerTick = 0.05;
		overheatSettings.heatGenerated = 0.05;

		weapon = UBio_Rifle::CreateBioRifle(inOwner);
		Cast<UBio_Rifle>(weapon)->SetSettings(overheatSettings);
		break;
	}
	case WeaponEnums::Bio_Repair: {
		weapon = UBio_Rifle::CreateBioRifle(inOwner);
		break;
	}
	case WeaponEnums::RPG: {
		FMagazineWeaponParams magSettings;
		magSettings.healthChange = 500;
		magSettings.range = 1300;
		magSettings.fireRate = 2.5;
		magSettings.heals = false;
		weapon = UWeapon::CreateWeapon(inOwner, magSettings);
		break;
	}
	case WeaponEnums::Shotgun: {
		weapon = UShotgun::CreateShotgun(inOwner);
		break;
	}
	case WeaponEnums::Sniper: {
		weapon = USniper::CreateSniper(inOwner);
		break;
	}
	case WeaponEnums::Sword: {
		FWeaponParams swordPrams;
		swordPrams.fireRate = 1.25;
		swordPrams.critChance = 70;
		swordPrams.damageType = EDamageType::Energy;
		swordPrams.healthChange = 100;
		swordPrams.range = UMiscLibrary::GetMeleeRange(inOwner);

		weapon = UWeapon::CreateWeapon(inOwner, swordPrams);
		break;
	}
	}

	weapon->SetGrade(weaponGrade);
	weapon->SetQuality(weaponQuality);

	if (weapon->GetName().IsEmpty())
	{
		weapon->SetName("Test Weapon");
	}

	return weapon;
}

TEnumAsByte<GameEnums::Difficulty> UMiscLibrary::GetDifficulty()
{
	return difficulty;
}

bool UMiscLibrary::IsCrit(float critChance)
{
	return IsSuccess(critChance);
}

bool UMiscLibrary::IsSuccess(float chance)
{
	return GetRandomPercent() <= chance;
}

float UMiscLibrary::GetRandomPercent()
{
	return rand() % 100;
}

float UMiscLibrary::GetMeleeRange(AMech_RPGCharacter* character)
{
	return (character->GetCapsuleComponent()->GetUnscaledCapsuleRadius() * 2) * 1.35;
}

FVector UMiscLibrary::FindNavLocation(AActor* actor, float radius)
{
	FNavLocation nav;
	int32 count = 0;
	bool locationFound = false;

	while (count < 120
		&& (!locationFound))
	{//|| !CanSee(actor->GetWorld(), actor->GetActorLocation(), nav.Location))) {
		radius *= 1.01;
		locationFound = actor->GetWorld()->GetNavigationSystem()->GetRandomReachablePointInRadius(actor->GetActorLocation(), radius, nav);
		count++;
	}

	return nav.Location;
}

template<class T>
void UMiscLibrary::InitialiseArray(TArray<T>& emptyArray, T* items)
{
	emptyArray.Append(items, ARRAY_COUNT(items));
}

FColor UMiscLibrary::GetRelativeColour(AMech_RPGCharacter* character)
{
	if (GetPlayer() != nullptr && character != nullptr)
	{
		if (character->CompareGroup(GetPlayer()))
		{
			return FColor::Green;
		}
		else
		{
			return FColor::Red;
		}
	}
	return FColor::Red;
}

void UMiscLibrary::SetDifficulty(GameEnums::Difficulty newDifficulty)
{
	difficulty = newDifficulty;
}

bool UMiscLibrary::CanSee(UWorld* world, FVector pointA, FVector pointB)
{
	TArray<FHitResult> results;
	world->LineTraceMultiByObjectType(results, pointA, pointB, mWorldCollision);
	return results.Num() == 0;
}

float UMiscLibrary::GetHealthPercent(AMech_RPGCharacter* character)
{
	return character != nullptr ? character->GetHealth() / character->GetMaxHealth() : 0.0;
}

float UMiscLibrary::GetMissingHealthPercent(AMech_RPGCharacter* character)
{
	return character != nullptr ? character->GetMaxHealth() / GetMissingHealth(character) : 0.0;
}

float UMiscLibrary::GetAbilityCooldownPercent(UAbility* ability)
{
	return ability != nullptr ? 1 - (ability->GetCurrentTimeRemaining() / ability->GetCooldown()) : 0.0;
}

float UMiscLibrary::GetAbilityChannelPercent(UChannelledAbility* ability)
{
	return ability != nullptr ? 1 - (ability->GetCurrentChannelTime() / ability->GetChannelDuration()) : 0.0;
}

bool UMiscLibrary::IsCharacterAlive(AMech_RPGCharacter* character)
{
	return character != nullptr && !character->IsDead();
}

bool UMiscLibrary::IsCharacterDead(AMech_RPGCharacter* character)
{
	return character != nullptr && character->IsDead();
}

AMech_RPGPlayerController* UMiscLibrary::GetPlayerController()
{
	return playerController;
}

void UMiscLibrary::SetPlayerController(AMech_RPGPlayerController* newController)
{
	playerController = newController;
}

void UMiscLibrary::OpenCharacterPane(UWorld* world)
{
	AMech_RPGCharacter* character = Cast<AMech_RPGCharacter>(UGameplayStatics::GetPlayerCharacter(world, 0));

	if (character != nullptr && character->GetController() != nullptr)
	{
		Cast<AMech_RPGPlayerController>(character->GetController())->OpenCharacterPane();
	}
}

UGroup* UMiscLibrary::GetPlayerGroup()
{
	/*if (GetPlayerController() != nullptr && GetPlayerController()->GetPlayerControllerOwner() != nullptr) {
		return GetPlayerController()->GetPlayerControllerOwner()->GetGroup();
	}*/
	if (playerGroup == nullptr)
	{
		UMiscLibrary::SetPlayerGroup(NewObject<UGroup>(UGroup::StaticClass()));
	}
	return playerGroup;
}

void UMiscLibrary::SetPlayerGroup(UGroup* group)
{
	playerGroup = group;
}

AMech_RPGCharacter* UMiscLibrary::GetPlayer()
{
	if (GetPlayerController() != nullptr)
	{
		return GetPlayerController()->GetPlayerControllerOwner();
	}
	return nullptr;
}

float UMiscLibrary::GetWidgetYaw(UCameraComponent* camera)
{

	if (camera != nullptr)
	{
		return camera->GetComponentRotation().Yaw + 90;
	}
	return 0;
}

UWorld* UMiscLibrary::GetActorWorld(AActor* actor)
{
	return actor != nullptr ? actor->GetWorld() : nullptr;
}

TArray<AMech_RPGCharacter*> UMiscLibrary::GetCharactersInRange(float range, AActor* origin)
{
	return UMiscLibrary::GetCharactersInRange(origin->GetWorld(), range, origin->GetActorLocation());
}

TArray<AMech_RPGCharacter*> UMiscLibrary::GetCharactersInRange(UWorld* world, float range, FVector location)
{
	TArray<AMech_RPGCharacter*> characters;
	for (TActorIterator<AMech_RPGCharacter> actor(world); actor; ++actor)
	{
		if ((actor->GetActorLocation() - location).Size() <= range)
		{
			characters.Add(*actor);
		}
	}

	return characters;
}

bool UMiscLibrary::IsMechCharacter(AActor* character)
{
	return mIsChildOf(character, AMech_RPGCharacter::StaticClass());
}

template<class T>
T* UMiscLibrary::SpawnCharacter(UWorld* world, FVector location, FRotator rotation, TSubclassOf<class AMech_RPGCharacter> classToSpawn)
{
	int count = 0;
	FNavLocation nav;
	AMech_RPGCharacter* character = nullptr;

	while (character == nullptr && count < 10)
	{
		FActorSpawnParameters params;
		params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
		world->GetNavigationSystem()->GetRandomPointInNavigableRadius(location, 100, nav);
		character = world->SpawnActor<T>(classToSpawn, nav.Location, rotation, params);
		count++;
	}

	if (character != nullptr)
	{
		character->SpawnDefaultController();
		return dynamic_cast<T*>(character);
	}
	else return nullptr;
}

bool UMiscLibrary::IsTargetValid(AMech_RPGCharacter* character, AMech_RPGCharacter* inTarget, EAffectedTeam affectedTeam)
{
	if (UMiscLibrary::IsCharacterAlive(inTarget) && UMiscLibrary::IsCharacterAlive(character))
	{
		if (affectedTeam == EAffectedTeam::Ally)
		{
			return character->IsAlly(inTarget);
		}
		else if (affectedTeam == EAffectedTeam::Enemy)
		{
			return character->IsEnemy(inTarget);
		}
		else if (affectedTeam == EAffectedTeam::Both)
		{
			return character->IsEnemy(inTarget) || character->IsAlly(inTarget);
		}
	}
	return false;
}


bool UMiscLibrary::IsChildOf(UObject* object, UClass* inClass)
{
	return object != nullptr ? object->GetClass()->IsChildOf(inClass) : false;
}

template<class T>
T UMiscLibrary::GetRandomEnum(T end)
{
	return static_cast<T>(rand() % ((int)end - 1));
}