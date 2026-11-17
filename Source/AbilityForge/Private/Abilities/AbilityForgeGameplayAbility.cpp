#include "Abilities/AbilityForgeGameplayAbility.h"

#include "AbilityForgeCharacter.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Core/AbilityForgeGameplayTags.h"
#include "GameplayEffect.h"

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

bool UAbilityForgeGameplayAbility::ApplyDamageToTarget(AActor* Target, float Damage) const
{
	UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo();
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);
	if (!DamageEffectClass || !SourceASC || !TargetASC)
	{
		return false;
	}

	const FGameplayEffectSpecHandle Spec = MakeOutgoingGameplayEffectSpec(DamageEffectClass, GetAbilityLevel());
	if (!Spec.IsValid())
	{
		return false;
	}

	// Negative, because the effect's modifier adds this value to Health.
	Spec.Data->SetSetByCallerMagnitude(AbilityForgeTags::Data_Damage, -Damage);

	SourceASC->ApplyGameplayEffectSpecToTarget(*Spec.Data.Get(), TargetASC);
	return true;
}
