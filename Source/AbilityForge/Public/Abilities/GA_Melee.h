#pragma once

#include "CoreMinimal.h"
#include "Abilities/AbilityForgeGameplayAbility.h"
#include "GameplayEffect.h"
#include "GA_Melee.generated.h"

class UAnimMontage;

/**
 * Plays an attack montage. At the impact frame an anim notify sends Event.Melee.Hit;
 * the server then traces in front of the character and damages what it hits.
 */
UCLASS()
class ABILITYFORGE_API UGA_Melee : public UAbilityForgeGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_Melee();

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Melee")
	TObjectPtr<UAnimMontage> AttackMontage;

	/** Damage per hit. Set Value to 1 and pick a curve table row to scale it with ability level. */
	UPROPERTY(EditDefaultsOnly, Category = "Melee")
	FScalableFloat Damage = FScalableFloat(10.f);

	UPROPERTY(EditDefaultsOnly, Category = "Melee")
	float TraceDistance = 150.f;

	UPROPERTY(EditDefaultsOnly, Category = "Melee")
	float TraceRadius = 50.f;

	/** Assignment 3: effect applied to each target hit, e.g. a short stun granting State.Stunned. */
	UPROPERTY(EditDefaultsOnly, Category = "Melee")
	TSubclassOf<UGameplayEffect> OnHitEffectClass;

	UPROPERTY(EditDefaultsOnly, Category = "Melee|Debug")
	bool bDrawDebugTrace = false;

private:
	UFUNCTION()
	void OnMontageFinished();

	UFUNCTION()
	void OnMontageCancelled();

	UFUNCTION()
	void OnHitEvent(FGameplayEventData Payload);

	void TraceForHits();

	void ExecuteHitCue(AActor* Target, const FHitResult& Hit) const;

	void ApplyOnHitEffect(AActor* Target) const;

	/** Actors already hit by this swing, so one swing never damages the same target twice. */
	UPROPERTY()
	TSet<TObjectPtr<AActor>> HitActors;
};
