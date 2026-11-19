#pragma once

#include "CoreMinimal.h"
#include "Abilities/AbilityForgeGameplayAbility.h"
#include "GA_Sprint.generated.h"

/** Assignment 1 solution. Raises walk speed while the input is held. */
UCLASS()
class ABILITYFORGE_API UGA_Sprint : public UAbilityForgeGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_Sprint();

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

	/** Called through AbilityLocalInputReleased when the sprint key is let go. */
	virtual void InputReleased(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo) override;

	virtual void EndAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		bool bReplicateEndAbility,
		bool bWasCancelled) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Sprint")
	float SpeedMultiplier = 1.75f;

private:
	float OriginalMaxWalkSpeed = 0.f;
	bool bSpeedApplied = false;
};
