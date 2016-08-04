
#pragma once
#include "Mech_RPG.h"
#include "BaseAIController.h"
#include "Characters/Mech_RPGCharacter.h"
#include "Weapons.h"

ABaseAIController::ABaseAIController() : Super() {
	PrimaryActorTick.bCanEverTick = true;
	SetActorTickEnabled(true);
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void ABaseAIController::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (GetOwner() && GetOwner()->GetDemandedController() == nullptr) {
		if (!GetOwner()->IsDead() && !GetOwner()->Channelling()) {
			bool abilityUsed = false;

			if (GetOwner()->CanCast()) {
				for (UAbility* ability : GetOwner()->GetAbilities()) {
					if (!ability->OnCooldown() && PerformAbility(ability)) {
						abilityUsed = true;
						break;
					}
				}
			}

			if (!abilityUsed) {
				if (GetOwner()->GetCurrentWeapon()->Heals()) {
					FindTarget(true);

					if (IsTargetValid(target, true) && UMiscLibrary::GetMissingHealth(target) > 0) {
						AttackTarget(DeltaTime);
					}
				}
				else {
					FindTarget(false);

					if (IsTargetValid(target, false)) {
						AttackTarget(DeltaTime);
					}
				}
			}

			if (!UMiscLibrary::IsCharacterAlive(GetTarget())) {
				GetOwner()->OnStopFiring.Broadcast();
			}
		}
	}
}

void ABaseAIController::OwnerPostBeginPlay(AMech_RPGCharacter* mech)
{
	GetOwner()->GetGroup()->OnMemberDamageEvent.AddUniqueDynamic(this, &ABaseAIController::GroupMemberDamaged);

}

void ABaseAIController::AttackTarget(float DeltaTime) {
	// Are we targeting ourselves
	if (target == GetOwner()) {
		//PerformAbility();
		FireWeapon(nullptr);
		GetOwner()->LookAt(target);
	}
	// Have we traced to another character or cover
	else if (UMiscLibrary::CanSee(GetWorld(), GetOwner()->GetActorLocation(), target->GetActorLocation())) {
		//PerformAbility();
		FireWeapon(target);
		GetOwner()->LookAt(target);
	}
	// We've hit some scenery so move towards the target
	else if (GetWorld()->GetNavigationSystem()) {
		MoveToLocation(target->GetActorLocation());
	}
}

UAbility* ABaseAIController::GetOwnerAbilityByTag(FString tag)
{
	if (!GetOwner()->Channelling()
		&& GetOwner()->CanCast()) {
		for (UAbility* ability : GetOwner()->GetAbilities()) {
			if (!ability->OnCooldown() && ability->HasTag(tag)) return ability;
		}
	}
	return nullptr;
}

void ABaseAIController::FireWeapon(AActor* hit) {
	AWeapon* weapon = GetOwner()->GetCurrentWeapon();
	float distToTarget = GetOwner()->GetDistanceTo(target);

	// Are we in weapons range
	if (weapon != nullptr && distToTarget <= weapon->GetRange()) {
		if (GetOwner()->CanAttack() && weapon->CanFire()) {

			bool isCover = hit != nullptr ? UMiscLibrary::IsCover(hit) : false;

			// Have we hit cover
			if (isCover) {
				float distToCover = GetOwner()->GetDistanceTo(hit);

				// Are we too far from the cover to avoid shooting it
				if (distToCover > 200) {
					weapon->Fire(Cast<ACover>(hit));
				}
				// Otherwise we can attack the target
				else {
					weapon->Fire(target);
				}
			}
			// Otherwise we've got a clear shot to the target
			else {
				weapon->Fire(target);

				if (!weapon->Heals()) {
					GetOwner()->GetGroup()->GroupMemberHit(GetOwner(), target);
				}
			}
		}

		SetFocus(target);
		StopMovement();
	}
	// We're out of range so move closer
	else if (GetWorld()->GetNavigationSystem()) {
		MoveToLocation(target->GetActorLocation());
	}
}

bool ABaseAIController::PerformAbility(UAbility* ability) {
	bool affectsAllies = ability->GetAffectedTeam() == AOEEnums::Ally;

	FindTarget(affectsAllies);

	if (IsTargetValid(target, affectsAllies) && ShouldHeal(ability) && ability->Activate(target, target->GetActorLocation())) {
		GetOwner()->SetCurrentAbility(ability);
		StopMovement();
		return true;
	}

	return false;
}

bool ABaseAIController::ShouldHeal(UAbility* ability) {
	return !ability->HasTag(UAbility::healTag) || ability->GetTagValue(UAbility::healTag) <= UMiscLibrary::GetMissingHealth(target);
}

void ABaseAIController::MoveToActor(AActor* target) {
	if (GetOwner()->CanMove()) {
		GetOwner()->OnStopFiring.Broadcast();
		GetWorld()->GetNavigationSystem()->SimpleMoveToActor(this, target);
	}
}

void ABaseAIController::MoveToLocation(FVector location) {
	if (GetOwner()->CanMove()) {
		GetOwner()->OnStopFiring.Broadcast();
		GetWorld()->GetNavigationSystem()->SimpleMoveToLocation(this, location);
	}
}

void ABaseAIController::FindTarget(bool ally) {
	float range = GetOwner()->GetCurrentWeapon()->GetRange();

	if (IsTargetValid(target, ally) && (!ally || UMiscLibrary::GetMissingHealth(target) > 0)) {
		return;
	}
	else {
		target = nullptr;
	}

	if (!ally) {
		if (target != nullptr && IsTargetValid(target = target->GetGroup()->GetRandomMember(), ally)) {
			SetTarget(target);
		}
		else {
			for (AMech_RPGCharacter* character : GetCharactersInRange(range)) {
				if (IsTargetValid(character, ally) && UMiscLibrary::CanSee(GetWorld(), GetOwner()->GetActorLocation(), character->GetActorLocation())) {
					if (character->GetGroup() != nullptr && character->GetGroup()->HasMemebers()) {
						SetTarget(character->GetGroup()->GetRandomMember());
					}
					else {
						SetTarget(character);
					}
					break;
				}
			}
		}
	}
	else {
		bool targetFound = false;

		AMech_RPGCharacter* character = GetOwner()->GetGroup()->GetLowHealthMember();
		if (IsTargetValid(character, ally)) {
			targetFound = true;
			SetTarget(character);
		}

		if (!targetFound) {
			for (AMech_RPGCharacter* character : GetCharactersInRange(range)) {
				if (IsTargetValid(character, ally) && UMiscLibrary::CanSee(GetWorld(), GetOwner()->GetActorLocation(), character->GetActorLocation())) {
					if (UMiscLibrary::GetMissingHealth(character) > 0) {
						SetTarget(character->GetGroup()->GetRandomMember());
						break;
					}
				}
			}
		}
	}
}

TArray<AMech_RPGCharacter*> ABaseAIController::GetCharactersInRange(float range) {
	return UMiscLibrary::GetCharactersInRange(range, GetOwner());
}

AMech_RPGCharacter* ABaseAIController::GetOwner() {
	return characterOwner;
}

AMech_RPGCharacter* ABaseAIController::GetTarget() {
	return target;
}

bool ABaseAIController::IsTargetValid(AMech_RPGCharacter* inTarget, bool ally) {
	bool valid = UMiscLibrary::IsCharacterAlive(inTarget) && ally == GetOwner()->CompareGroup(inTarget);
	
	return valid;
}

void ABaseAIController::SetOwner(AMech_RPGCharacter* newVal) {
	if (characterOwner != nullptr && characterOwner->GetGroup() != nullptr) {
		characterOwner->GetGroup()->OnMemberDamageEvent.RemoveDynamic(this, &ABaseAIController::GroupMemberDamaged);
	}

	if (newVal != nullptr) {
		newVal->OnPostBeginPlay.AddUniqueDynamic(this, &ABaseAIController::OwnerPostBeginPlay);

		if (newVal->GetGroup() != nullptr) {
			newVal->GetGroup()->OnMemberDamageEvent.AddUniqueDynamic(this, &ABaseAIController::GroupMemberDamaged);
		}
	}

	characterOwner = newVal;
}

void ABaseAIController::SetTarget(AMech_RPGCharacter* newVal) {
	target = newVal;
}

void ABaseAIController::GroupMemberDamaged(AMech_RPGCharacter* attacker, AMech_RPGCharacter* damagedMember) {
	if (!UMiscLibrary::IsCharacterAlive(GetTarget())) {
		SetTarget(attacker);
	}
}