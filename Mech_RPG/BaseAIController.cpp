
#pragma once
#include "Mech_RPG.h"
#include "BaseAIController.h"
#include "Characters/Mech_RPGCharacter.h"
#include "Weapons.h"
#include "Quests/QuestManager.h"

ABaseAIController::ABaseAIController() : Super()
{
	PrimaryActorTick.bCanEverTick = true;
	SetActorTickEnabled(true);
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void ABaseAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (UMiscLibrary::IsCharacterAlive(GetAIOwner())
		&& GetAIOwner()->GetDemandedController() == nullptr
		&& !GetAIOwner()->Channelling())
	{
		bool abilityUsed = false;

		if (GetAIOwner()->CanCast())
		{
			for (UAbility* ability : GetAIOwner()->GetAbilities())
			{
				if (!ability->OnCooldown() && PerformAbility(ability))
				{
					abilityUsed = true;
					UQuestManager::AbilityUsed(ability);
					break;
				}
			}
		}

		if (!abilityUsed)
		{
			if (GetAIOwner()->GetCurrentWeapon()->Heals())
			{
				FindTarget(EAffectedTeam::Ally);

				if (targetCharacter != nullptr)
				{
					UseWeaponOnTarget(DeltaTime);
				}
			}
			else
			{
				FindTarget(EAffectedTeam::Enemy);

				if (targetCharacter != nullptr)
				{
					UseWeaponOnTarget(DeltaTime);
				}
			}
		}

		if (!UMiscLibrary::IsCharacterAlive(GetTarget()))
		{
			GetAIOwner()->OnStopFiring.Broadcast();
		}
	}
}

void ABaseAIController::Possess(APawn* InPawn)
{
	Super::Possess(InPawn);
	if (UMiscLibrary::IsMechCharacter(InPawn))
	{
		SetAIOwner(Cast<AMech_RPGCharacter>(InPawn));
	}
}

void ABaseAIController::OwnerPostBeginPlay(AMech_RPGCharacter* mech)
{
	GetAIOwner()->GetGroup()->OnMemberDamageEvent.AddUniqueDynamic(this, &ABaseAIController::GroupMemberDamaged);
}

void ABaseAIController::UseWeaponOnTarget(float DeltaTime)
{
	// Are we targeting ourselves
	if (targetCharacter == GetAIOwner())
	{
		UseWeapon(nullptr);
		SetFocus(targetCharacter);
	}
	// Have we traced to another character or cover
	else if (mCanSee(targetCharacter))
	{
		UseWeapon(targetCharacter);
		SetFocus(targetCharacter);
	}
	// We've hit some scenery so move towards the target
	else if (GetWorld()->GetNavigationSystem())
	{
		MoveToLocation(targetCharacter->GetActorLocation());
	}
}

UAbility* ABaseAIController::GetOwnerAbilityByTag(FString tag)
{
	if (!GetAIOwner()->Channelling()
		&& GetAIOwner()->CanCast())
	{
		for (UAbility* ability : GetAIOwner()->GetAbilities())
		{
			if (!ability->OnCooldown() && ability->HasTag(tag)) return ability;
		}
	}
	return nullptr;
}

void ABaseAIController::UseWeapon(AActor* hit)
{
	AWeapon* weapon = GetAIOwner()->GetCurrentWeapon();
	float distToTarget = GetAIOwner()->GetDistanceTo(targetCharacter);

	// Are we in weapons range
	bool isInRange = weapon != nullptr && distToTarget <= weapon->GetRange();

	if (isInRange)
	{
		bool canFire = GetAIOwner()->CanAttack() && weapon->CanFire();

		if (canFire)
		{
			weapon->UseWeapon(targetCharacter);

			if (!weapon->Heals())
			{
				GetAIOwner()->GetGroup()->GroupMemberHit(GetAIOwner(), targetCharacter);
			}
		}

		SetFocus(targetCharacter);
		StopMovement();
	}
	// We're out of range so move closer
	else if (GetWorld()->GetNavigationSystem())
	{
		MoveToLocation(targetCharacter->GetActorLocation());
	}
}

bool ABaseAIController::PerformAbility(UAbility* ability)
{
	FindTarget(ability->GetAffectedTeam());

	if (targetCharacter != nullptr
		&& ability->Activate(targetCharacter, targetCharacter->GetActorLocation()))
	{
		GetAIOwner()->SetCurrentAbility(ability);
		StopMovement();
		GetAIOwner()->OnStopFiring.Broadcast();
		return true;
	}

	return false;
}

bool ABaseAIController::ShouldHeal(UAbility* ability)
{
	return !ability->HasTag(UAbility::healTag) || ability->GetTagValue(UAbility::healTag) <= UMiscLibrary::GetMissingHealth(targetCharacter);
}

void ABaseAIController::MoveToActor(AActor* target)
{
	if (GetAIOwner()->CanMove())
	{
		GetAIOwner()->OnStopFiring.Broadcast();
		GetWorld()->GetNavigationSystem()->SimpleMoveToActor(this, target);
	}
}

void ABaseAIController::MoveToLocation(FVector location)
{
	if (GetAIOwner()->CanMove())
	{
		GetAIOwner()->OnStopFiring.Broadcast();
		GetWorld()->GetNavigationSystem()->SimpleMoveToLocation(this, location);
	}
}

void ABaseAIController::FindTarget(EAffectedTeam affectedTeam)
{
	float range = GetAIOwner()->GetCurrentWeapon()->GetRange();
	bool isAffectedTeamEnemy = affectedTeam == EAffectedTeam::Enemy;

	// Is the target
	if (IsTargetValid(targetCharacter, affectedTeam)

		// If they're an enemy then we can carry on, or only affect allies that have lost health
		&& (isAffectedTeamEnemy || mLostHealth(targetCharacter)))
	{
		return;
	}
	// Otherwise remove the target character and find a new one
	else
	{
		targetCharacter = nullptr;
	}

	if (isAffectedTeamEnemy)
	{
		for (AMech_RPGCharacter* character : GetCharactersInRange(range))
		{
			if (IsTargetValid(character, affectedTeam) && mCanSee(character))
			{
				SetTarget(character);
				break;
			}
		}
	}
	else
	{
		// Will only return a member if their health lost is 1 or more
		AMech_RPGCharacter* lowHealthCharacter = GetAIOwner()->GetGroup()->GetLowHealthMember();

		// Did we find a group member to heal
		if (lowHealthCharacter != nullptr)
		{
			SetTarget(lowHealthCharacter);
		}
		else
		{
			// Search for allies outside of the group within range
			for (AMech_RPGCharacter* character : GetCharactersInRange(range))
			{

				// Is the target valid
				if (IsTargetValid(character, affectedTeam))
				{

					// Have they lost health
					if (mLostHealth(character))
					{
						SetTarget(character);
						break;
					}
				}
			}
		}
	}
}

TArray<AMech_RPGCharacter*> ABaseAIController::GetCharactersInRange(float range)
{
	return UMiscLibrary::GetCharactersInRange(range, GetAIOwner());
}

AMech_RPGCharacter* ABaseAIController::GetAIOwner()
{
	return characterOwner;
}

AMech_RPGCharacter* ABaseAIController::GetTarget()
{
	return targetCharacter;
}

bool ABaseAIController::IsTargetValid(AMech_RPGCharacter* inTarget, EAffectedTeam affectedTeam)
{
	return UMiscLibrary::IsTargetValid(GetAIOwner(), inTarget, affectedTeam);
}

void ABaseAIController::SetAIOwner(AMech_RPGCharacter* newVal)
{
	if (characterOwner != nullptr && characterOwner->GetGroup() != nullptr)
	{
		characterOwner->GetGroup()->OnMemberDamageEvent.RemoveDynamic(this, &ABaseAIController::GroupMemberDamaged);
	}

	if (newVal != nullptr)
	{
		newVal->OnPostBeginPlay.AddUniqueDynamic(this, &ABaseAIController::OwnerPostBeginPlay);

		if (newVal->GetGroup() != nullptr)
		{
			newVal->GetGroup()->OnMemberDamageEvent.AddUniqueDynamic(this, &ABaseAIController::GroupMemberDamaged);
		}
	}

	characterOwner = newVal;
}

void ABaseAIController::SetTarget(AMech_RPGCharacter* newVal)
{
	targetCharacter = newVal;
}

void ABaseAIController::GroupMemberDamaged(AMech_RPGCharacter* attacker, AMech_RPGCharacter* damagedMember)
{
	if (!UMiscLibrary::IsCharacterAlive(GetTarget()))
	{
		SetTarget(attacker);
	}
}
