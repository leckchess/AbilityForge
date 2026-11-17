// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "AbilityForgeCharacter.generated.h"

class UAbilitySystemComponent;

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

private:
	void InitAbilityActorInfo();

	/** Owned by the PlayerState; cached here once actor info is initialized. */
	TWeakObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
};
