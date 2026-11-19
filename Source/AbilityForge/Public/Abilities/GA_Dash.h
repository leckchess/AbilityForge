#pragma once

#include "CoreMinimal.h"
#include "Abilities/AbilityForgeGameplayAbility.h"
#include "GA_Dash.generated.h"

class UAnimMontage;

/** Launches the character forward. Only works on the ground. */
UCLASS()
class ABILITYFORGE_API UGA_Dash : public UAbilityForgeGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_Dash();

	/** Adds a custom rule on top of tags, cost and cooldown: no dashing in the air. */
	virtual bool CanActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayTagContainer* SourceTags = nullptr,
		const FGameplayTagContainer* TargetTags = nullptr,
		FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		bool bReplicateEndAbility,
		bool bWasCancelled) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Dash")
	float DashStrength = 1500.f;

	/** How long the ability stays active (and State.Dashing stays applied) after the launch. */
	UPROPERTY(EditDefaultsOnly, Category = "Dash")
	float DashDuration = 0.25f;

	/** Optional. If set, the dash plays it and ends when it finishes, instead of after DashDuration. */
	UPROPERTY(EditDefaultsOnly, Category = "Dash")
	TObjectPtr<UAnimMontage> DashMontage;

private:
	void FinishDash();

	UFUNCTION()
	void OnDashMontageFinished();

	UFUNCTION()
	void OnDashMontageCancelled();

	FTimerHandle DashTimerHandle;
};
