#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AbilityForgeGameplayAbility.generated.h"

class AAbilityForgeCharacter;

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
};
