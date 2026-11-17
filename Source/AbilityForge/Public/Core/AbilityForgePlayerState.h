#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "AbilityForgePlayerState.generated.h"

class UAbilitySystemComponent;
class UAbilityForgeAttributeSet;

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

	UAbilityForgeAttributeSet* GetAttributeSet() const { return AttributeSet; }

	UFUNCTION(BlueprintPure, Category = "AbilityForge")
	int32 GetCharacterLevel() const { return CharacterLevel; }

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilityForge")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAbilityForgeAttributeSet> AttributeSet;

	/** Level abilities are granted at. Melee damage scales with it through the damage curve. */
	UPROPERTY(EditDefaultsOnly, Replicated, BlueprintReadOnly, Category = "AbilityForge", meta = (ClampMin = "1"))
	int32 CharacterLevel = 1;
};
