#include "Abilities/GA_Sprint.h"

#include "Core/AbilityForgeGameplayTags.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UGA_Sprint::UGA_Sprint()
{
	FGameplayTagContainer SprintAssetTags;
	SprintAssetTags.AddTag(AbilityForgeTags::Ability_Sprint);
	SetAssetTags(SprintAssetTags);

	ActivationOwnedTags.AddTag(AbilityForgeTags::State_Sprinting);
	ActivationBlockedTags.AddTag(AbilityForgeTags::State_Stunned);
	ActivationBlockedTags.AddTag(AbilityForgeTags::State_Attacking);
}

void UGA_Sprint::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	const ACharacter* Character = ActorInfo ? Cast<ACharacter>(ActorInfo->AvatarActor.Get()) : nullptr;
	if (!Character || !CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	// Runs on both the predicting client and the server, so their movement stays in sync.
	UCharacterMovementComponent* Movement = Character->GetCharacterMovement();
	OriginalMaxWalkSpeed = Movement->MaxWalkSpeed;
	Movement->MaxWalkSpeed = OriginalMaxWalkSpeed * SpeedMultiplier;
	bSpeedApplied = true;
}

void UGA_Sprint::InputReleased(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo)
{
	if (IsActive())
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	}
}

void UGA_Sprint::EndAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility,
	bool bWasCancelled)
{
	if (bSpeedApplied)
	{
		if (const ACharacter* Character = ActorInfo ? Cast<ACharacter>(ActorInfo->AvatarActor.Get()) : nullptr)
		{
			Character->GetCharacterMovement()->MaxWalkSpeed = OriginalMaxWalkSpeed;
		}
		bSpeedApplied = false;
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
