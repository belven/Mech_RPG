// Fill out your copyright notice in the Description page of Project Settings.

#include "Mech_RPG.h"
#include "Abilities/ChannelledAbility.h"
#include "Characters/Mech_RPGCharacter.h"
#include "DrawDebugHelpers.h"

UChannelledAbility::UChannelledAbility() : Super() {
	partclSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ChannelLineParticleSystem"));

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleSystemClass(TEXT("/Game/TopDown/Particle_Effects/AoE"));
	if (ParticleSystemClass.Succeeded()) {
		partclSystem->Template = ParticleSystemClass.Object;
		partclSystem->bAutoActivate = false;
	}
}

bool UChannelledAbility::Activate(class AMech_RPGCharacter* target, FVector inTargetLocation) {
	if (!channelling) {
		channelling = true;
		owner->SetChannelling(true);
		currentChannelTime = channelDuration - 0.1F;
		owner->ApplyCrowdControl(EffectEnums::Move, false);
		owner->ApplyCrowdControl(EffectEnums::Attack, false);
		targetCharacter = target;
		targetLocation = inTargetLocation;
		owner->GetWorld()->GetTimerManager().SetTimer(TimerHandle_AbilityOffCooldown, this, &UChannelledAbility::ActiveChannelAbility, 0.1F);
		return true;
	}
	return false;
}

bool UChannelledAbility::Channelling() {
	return channelling;
}

float UChannelledAbility::GetCooldown() {
	return abilityToActivate->GetCooldown();
}

float UChannelledAbility::GetChannelDuration() {
	float percentChange = 1 - (owner->GetSpeedModifier() - 1);
	return channelDuration * percentChange;
}

float UChannelledAbility::GetCurrentChannelTime() {
	return currentChannelTime;
}

bool UChannelledAbility::OnCooldown() {
	return abilityToActivate->OnCooldown();
}

bool UChannelledAbility::HasTag(FString name)
{
	return abilityToActivate->HasTag(name);
}

TArray<FTag>& UChannelledAbility::GetTags()
{
	return abilityToActivate->GetTags();
}

FTag UChannelledAbility::GetTag(FString name)
{
	return abilityToActivate->GetTag(name);
}

float UChannelledAbility::GetTagValue(FString name)
{
	return abilityToActivate->GetTagValue(name);
}

bool UChannelledAbility::GetTagTrue(FString name)
{
	return abilityToActivate->GetTagTrue(name);
}

float UChannelledAbility::GetCurrentTimeRemaining() {
	return abilityToActivate->GetCurrentTimeRemaining();
}

FString UChannelledAbility::GetTooltipText()
{
	return abilityToActivate->GetTooltipText();
}

void UChannelledAbility::ActiveChannelAbility() {
	bool ownerIsValid = UMiscLibrary::IsCharacterAlive(owner) && owner->Channelling() && owner->CanCast();
	bool targetIsValid = usesLocation ? true : UMiscLibrary::IsCharacterAlive(targetCharacter);
	bool reset = true;

	if (!usesLocation && targetIsValid) {
		targetLocation = targetCharacter->GetActorLocation();
	}

	if (channelling	&& ownerIsValid && targetIsValid) {
		if (currentChannelTime > 0) {
			reset = false;
			currentChannelTime -= 0.1F;

			if (abilityToActivate->HasTag(UAbility::aoeTag)) {

				//FMatrix TM;
				//TM.SetOrigin(targetLocation);
				//TM.SetAxis(0, FVector(1, 0, 1)); //x
				//TM.SetAxis(1, FVector(0, 1, 1)); //y
				//TM.SetAxis(2, FVector(0, 0, 1)); //z
				//DrawDebugCircle(owner->GetWorld(), TM, abilityToActivate->GetTagValue(UAbility::aoeTag), 40, UMiscLibrary::GetRelativeColour(owner), false, 0.1F, 0, 5, false);
				DrawDebugSphere(owner->GetWorld(), targetLocation, abilityToActivate->GetTagValue(UAbility::aoeTag), 10, UMiscLibrary::GetRelativeColour(owner), false, 0.1F, 0);
			}

			DrawDebugLine(owner->GetWorld(), owner->GetActorLocation(), targetLocation, UMiscLibrary::GetRelativeColour(owner), false, 0.1F, 0, 10);
			owner->GetWorld()->GetTimerManager().SetTimer(TimerHandle_AbilityOffCooldown, this, &UChannelledAbility::ActiveChannelAbility, 0.1F);
		}
		else if (!usesTrace || targetCharacter == owner || (usesTrace && PerformLineTrace())) {
			abilityToActivate->Activate(targetCharacter, targetLocation);
		}
		else if (usesTrace) {
			abilityToActivate->SetOnCooldown(owner->GetWorld());
		}
	}

	if (reset) {
		owner->SetChannelling(false);
		channelling = false;
		currentChannelTime = 0;
		owner->ApplyCrowdControl(EffectEnums::Move, true);
		owner->ApplyCrowdControl(EffectEnums::Attack, true);
		targetCharacter = nullptr;
		targetLocation = FVector::ZeroVector;
	}
}

bool UChannelledAbility::PerformLineTrace() {
	collision.ClearIgnoredComponents();

	if (owner->GetGroup() != nullptr && owner->GetGroup()->HasMemebers()) {
		for (AMech_RPGCharacter* member : owner->GetGroup()->GetMembers()) {
			if (member != targetCharacter) {
				collision.AddIgnoredActor(member);
			}
		}
	}

	owner->GetWorld()->LineTraceSingleByObjectType(hit, owner->GetActorLocation(), targetLocation, objectCollision, collision);

	bool targetTraced = hit.bBlockingHit && hit.GetActor() != nullptr;

	if (targetTraced && UMiscLibrary::IsMechCharacter(hit.GetActor())) {
		targetCharacter = Cast<AMech_RPGCharacter>(hit.GetActor());		
		return UMiscLibrary::IsTargetValid(owner, targetCharacter, abilityToActivate->GetAffectedTeam());
	}

	return false;
}

/*
Activates an ability at the end of a set duration, providing that:

1. The owner is still alive
2. The owner can cast and channelling is TRUE
3. If inUsesTrace is true and the line trace detects a viable target

Note: inUsesLocation will cause this to fixate on the original location that was passed in, i.e. the cursor location.
*/
UChannelledAbility* UChannelledAbility::CreateChannelledAbility(AMech_RPGCharacter* inOwner, UAbility* inAbilityToActivate, float inChannelDuration, bool inUsesLocation, bool inUsesTrace) {
	UChannelledAbility* ability = NewObject<UChannelledAbility>(StaticClass());
	ability->objectCollision.AddObjectTypesToQuery(mWorldCollision);
	ability->objectCollision.AddObjectTypesToQuery(mCharacterCollision);
	ability->owner = inOwner;
	ability->abilityToActivate = inAbilityToActivate;
	ability->channelDuration = inChannelDuration;
	ability->usesLocation = inUsesLocation;
	ability->usesTrace = inUsesTrace;
	return ability;
}