#include "Abilities/AbilityForgeGameplayAbility.h"

#include "AbilityForgeCharacter.h"

UAbilityForgeGameplayAbility::UAbilityForgeGameplayAbility()
{
	// One ability object per actor, so it can keep state (timers, hit lists) while active.
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	// Runs on the owning client immediately; the server runs it too and confirms or rejects it.
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

AAbilityForgeCharacter* UAbilityForgeGameplayAbility::GetAbilityForgeCharacter() const
{
	return Cast<AAbilityForgeCharacter>(GetAvatarActorFromActorInfo());
}
