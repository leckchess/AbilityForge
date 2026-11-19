// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilityForgeCharacter.h"

#include "AbilitySystemComponent.h"
#include "Core/AbilityForgePlayerState.h"

UAbilitySystemComponent* AAbilityForgeCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
}

void AAbilityForgeCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitAbilityActorInfo();
}

void AAbilityForgeCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	InitAbilityActorInfo();
}

void AAbilityForgeCharacter::InitAbilityActorInfo()
{
	AAbilityForgePlayerState* AFPlayerState = GetPlayerState<AAbilityForgePlayerState>();
	if (!AFPlayerState)
	{
		if (GetPlayerState())
		{
			UE_LOG(LogTemp, Warning, TEXT("%s: set the Game Mode's Player State Class to AbilityForgePlayerState."), *GetName());
		}
		// OnRep_PlayerState also fires when the PlayerState is cleared.
		return;
	}

	AbilitySystemComponent = AFPlayerState->GetAbilitySystemComponent();

	// Owner = the PlayerState, which keeps the ASC. Avatar = this character, the body in the world.
	AbilitySystemComponent->InitAbilityActorInfo(AFPlayerState, this);
}
