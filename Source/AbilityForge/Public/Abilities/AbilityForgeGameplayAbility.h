#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AbilityForgeGameplayAbility.generated.h"

class AAbilityForgeCharacter;
class UGameplayEffect;

/** Base class for every AbilityForge ability. Sets shared defaults and helpers. */
UCLASS(Abstract)
class ABILITYFORGE_API UAbilityForgeGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UAbilityForgeGameplayAbility();

protected:
	UFUNCTION(BlueprintPure, Category = "Ability")
	AAbilityForgeCharacter* GetAbilityForgeCharacter() const;

	/** Effect used for damage. Its Health modifier reads the Data.Damage SetByCaller magnitude. */
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	/** Server only. Applies DamageEffectClass to Target, passing Damage as SetByCaller. */
	bool ApplyDamageToTarget(AActor* Target, float Damage) const;
};
