// Fill out your copyright notice in the Description page of Project Settings.

#include "Mech_RPG.h"
#include "ChannelledAbility.h"
#include "Mech_RPGCharacter.h"
#include "DrawDebugHelpers.h"

void UChannelledAbility::Activate(AMech_RPGCharacter* target, FVector targetLocation) {
	if (!channelling) {
		channelling = true;
		owner->SetChannelling(true);
		currentChannelTime = channelDuration - 0.1F;
		owner->ApplyCrowdControl(EffectEnums::Move, false);
		owner->ApplyCrowdControl(EffectEnums::Attack, false);
		targetCharacter = target;
		this->targetLocation = targetLocation != FVector::ZeroVector ? targetLocation : target->GetActorLocation();
		owner->GetWorld()->GetTimerManager().SetTimer(TimerHandle_AbilityOffCooldown, this, &UChannelledAbility::ActiveChannelAbility, 0.1F);
	}
}

bool UChannelledAbility::Channelling() {
	return channelling;
}

float UChannelledAbility::GetCooldown() {
	return abilityToActivate->GetCooldown();
}

float UChannelledAbility::GetChannelDuration() {
	return channelDuration;
}

float UChannelledAbility::GetCurrentChannelTime() {
	return currentChannelTime;
}

bool UChannelledAbility::OnCooldown() {
	return abilityToActivate->OnCooldown();
}

float UChannelledAbility::GetCurrentTimeRemaining() {
	return abilityToActivate->GetCurrentTimeRemaining();
}

void UChannelledAbility::ActiveChannelAbility() {
	bool ownerIsValid = !owner->IsDead() && owner->Channelling() && owner->CanCast();
	bool targetIsValid = usesLocation ? true : targetCharacter != NULL && !targetCharacter->IsDead();
	bool reset = false;

	targetLocation = !usesLocation && targetIsValid ? targetCharacter->GetActorLocation() : targetLocation;

	if (channelling	&& ownerIsValid && targetIsValid) {
		if (currentChannelTime > 0) {
			currentChannelTime -= 0.1F;
			DrawDebugLine(owner->GetWorld(), owner->GetActorLocation(), targetLocation, FColor::Blue, false, 0.2, 0, 5);
			owner->GetWorld()->GetTimerManager().SetTimer(TimerHandle_AbilityOffCooldown, this, &UChannelledAbility::ActiveChannelAbility, 0.1F);
		}
		else if (!usesTrace || (usesTrace && PerformLineTrace())) {
			abilityToActivate->Activate(targetCharacter, targetLocation);
			reset = true;
		}
		else {
			reset = true;
		}
	}
	else {
		reset = true;
	}

	if (reset) {
		owner->SetChannelling(false);
		channelling = false;
		currentChannelTime = 0;
		owner->ApplyCrowdControl(EffectEnums::Move, true);
		owner->ApplyCrowdControl(EffectEnums::Attack, true);
		targetCharacter = NULL;
		targetLocation = FVector::ZeroVector;
	}
}

bool UChannelledAbility::PerformLineTrace() {
	collision.IgnoreComponents.Empty();

	if (owner->GetGroup() != NULL && owner->GetGroup()->HasMemebers()) {
		for (AMech_RPGCharacter* member : owner->GetGroup()->GetMembers()) {
			if (member != targetCharacter) {
				collision.AddIgnoredActor(member);
			}
		}
	}

	owner->GetWorld()->LineTraceSingleByObjectType(hit, owner->GetActorLocation(), targetLocation, objectCollision, collision);

	bool targetTraced = hit.bBlockingHit && hit.GetActor() != NULL;
	bool affectsAllies = abilityToActivate->GetAffectedTeam() == AOEEnums::Ally;

	if (targetTraced && (hit.GetActor()->GetClass()->IsChildOf(AMech_RPGCharacter::StaticClass()))) {
		AMech_RPGCharacter* tempCharacter = Cast<AMech_RPGCharacter>(hit.GetActor());
		targetCharacter = tempCharacter;
		return affectsAllies && tempCharacter->team == owner->team || !affectsAllies && tempCharacter->team != owner->team;
	}
	else if (targetTraced && hit.GetActor()->GetClass()->IsChildOf(ACover::StaticClass())) {
		//	ACover* tempCover = Cast<ACover>(hit.GetActor());
		return true;
	}

	return false;
	//float distToCover = FVector::Dist(owner->GetActorLocation(), hit->GetActorLocation());
}

UChannelledAbility* UChannelledAbility::CreateChannelledAbility(AMech_RPGCharacter* inOwner, UAbility* inAbilityToActivate, float inChannelDuration, bool inUsesLocation, bool inUsesTrace) {
	UChannelledAbility* ability = NewObject<UChannelledAbility>(StaticClass());
	ability->objectCollision.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);
	ability->objectCollision.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);
	ability->owner = inOwner;
	ability->abilityToActivate = inAbilityToActivate;
	ability->channelDuration = inChannelDuration;
	ability->usesLocation = inUsesLocation;
	ability->usesTrace = inUsesTrace;
	return ability;
}