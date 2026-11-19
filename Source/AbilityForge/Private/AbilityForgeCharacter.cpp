// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilityForgeCharacter.h"

#include "AbilitySystemComponent.h"
#include "Core/AbilityForgePlayerState.h"
#include "Abilities/GameplayAbility.h"
#include "EnhancedInputComponent.h"

UAbilitySystemComponent* AAbilityForgeCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
}

void AAbilityForgeCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitAbilityActorInfo();
	GrantStartupAbilities();
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

void AAbilityForgeCharacter::GrantStartupAbilities()
{
	// Abilities are only granted on the server; specs replicate to the owning client.
	if (!HasAuthority() || !AbilitySystemComponent.IsValid())
	{
		return;
	}

	const AAbilityForgePlayerState* AFPlayerState = GetPlayerState<AAbilityForgePlayerState>();
	const int32 Level = AFPlayerState ? AFPlayerState->GetCharacterLevel() : 1;

	for (int32 Index = 0; Index < StartupAbilities.Num(); ++Index)
	{
		const TSubclassOf<UGameplayAbility> AbilityClass = StartupAbilities[Index].Ability;

		// Skip empty entries, and abilities the PlayerState's ASC already has from a previous character.
		if (!AbilityClass || AbilitySystemComponent->FindAbilitySpecFromClass(AbilityClass))
		{
			continue;
		}

		// The binding's index is used as the spec's InputID.
		AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(AbilityClass, Level, Index, this));
	}
}

void AAbilityForgeCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Movement and camera are bound in BP_ThirdPersonCharacter. Only ability input is bound here.
	UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (!EnhancedInput)
	{
		return;
	}

	for (int32 Index = 0; Index < StartupAbilities.Num(); ++Index)
	{
		if (const UInputAction* Action = StartupAbilities[Index].InputAction)
		{
			EnhancedInput->BindAction(Action, ETriggerEvent::Started, this, &AAbilityForgeCharacter::OnAbilityInputPressed, Index);
			EnhancedInput->BindAction(Action, ETriggerEvent::Completed, this, &AAbilityForgeCharacter::OnAbilityInputReleased, Index);
		}
	}
}

void AAbilityForgeCharacter::OnAbilityInputPressed(int32 InputID)
{
	// Activates the ability with this InputID, or forwards the press if it is already active.
	if (AbilitySystemComponent.IsValid())
	{
		AbilitySystemComponent->AbilityLocalInputPressed(InputID);
	}
}

void AAbilityForgeCharacter::OnAbilityInputReleased(int32 InputID)
{
	// Calls InputReleased on the active ability with this InputID.
	if (AbilitySystemComponent.IsValid())
	{
		AbilitySystemComponent->AbilityLocalInputReleased(InputID);
	}
}
