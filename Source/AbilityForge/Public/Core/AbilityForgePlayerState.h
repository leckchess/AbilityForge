#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "AbilityForgePlayerState.generated.h"

class UAbilitySystemComponent;

/**
 * Owns the Ability System Component, so abilities, effects and attributes
 * survive the character being destroyed and respawned.
 */
UCLASS()
class ABILITYFORGE_API AAbilityForgePlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AAbilityForgePlayerState();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilityForge")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
};
