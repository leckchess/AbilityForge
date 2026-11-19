// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "AbilityForgeCharacter.generated.h"

class UAbilitySystemComponent;
class UGameplayAbility;
class UInputAction;
class UGameplayEffect;

/** An ability granted at startup, and the input action that activates it. */
USTRUCT(BlueprintType)
struct FAbilityForgeAbilityBinding
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TSubclassOf<UGameplayAbility> Ability;

	/** Optional. Leave empty for abilities that are activated by events or code. */
	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TObjectPtr<UInputAction> InputAction;
};

/**
 * Player character. Movement, camera and template input stay in BP_ThirdPersonCharacter;
 * this class only connects the character to the Gameplay Ability System.
 */
UCLASS()
class ABILITYFORGE_API AAbilityForgeCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	/** Server: a controller has possessed this character. */
	virtual void PossessedBy(AController* NewController) override;

	/** Clients: the PlayerState, which owns the ASC, has replicated. */
	virtual void OnRep_PlayerState() override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

protected:
	/** Abilities granted on the server when the character is first possessed. */
	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TArray<FAbilityForgeAbilityBinding> StartupAbilities;

	/** Instant effect that sets the starting Health and Stamina. */
	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TSubclassOf<UGameplayEffect> DefaultAttributesEffect;

	/** Effects applied once at startup, for example stamina regeneration. */
	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TArray<TSubclassOf<UGameplayEffect>> StartupEffects;

private:
	void InitAbilityActorInfo();

	void GrantStartupAbilities();
	void OnAbilityInputPressed(int32 InputID);
	void OnAbilityInputReleased(int32 InputID);

	void ApplyStartupEffects();

	/** Owned by the PlayerState; cached here once actor info is initialized. */
	TWeakObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
};
