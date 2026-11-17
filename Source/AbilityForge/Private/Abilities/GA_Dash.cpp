#include "Abilities/GA_Dash.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"
#include "Core/AbilityForgeGameplayTags.h"

UGA_Dash::UGA_Dash()
{
	FGameplayTagContainer DashAssetTags;
	DashAssetTags.AddTag(AbilityForgeTags::Ability_Dash);
	SetAssetTags(DashAssetTags);

	// Applied to the owner while the dash is active.
	ActivationOwnedTags.AddTag(AbilityForgeTags::State_Dashing);

	// The dash can't start while the owner has any of these.
	ActivationBlockedTags.AddTag(AbilityForgeTags::State_Stunned);
	ActivationBlockedTags.AddTag(AbilityForgeTags::State_Attacking);
}

bool UGA_Dash::CanActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags,
	FGameplayTagContainer* OptionalRelevantTags) const
{
	// Super checks tags, cost and cooldown. Always call it first.
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	// Use ActorInfo, not member variables: this can run on the class default object.
	const ACharacter* Character = ActorInfo ? Cast<ACharacter>(ActorInfo->AvatarActor.Get()) : nullptr;

	return Character && !Character->GetCharacterMovement()->IsFalling();
}

void UGA_Dash::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	// Commit applies the cost and cooldown. If they can't be paid, end without dashing.
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	if (!Character)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	// Forward is the same on client and server, so the predicted launch matches the server's.
	const FVector Direction = Character->GetActorForwardVector().GetSafeNormal2D();
	Character->LaunchCharacter(Direction * DashStrength, true, false);

	GetWorld()->GetTimerManager().SetTimer(DashTimerHandle, this, &UGA_Dash::FinishDash, DashDuration, false);
}

void UGA_Dash::EndAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility,
	bool bWasCancelled)
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(DashTimerHandle);
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_Dash::FinishDash()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
