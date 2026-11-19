#include "Core/AbilityForgePlayerState.h"

#include "AbilitySystemComponent.h"

AAbilityForgePlayerState::AAbilityForgePlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);

	// Mixed: the owning client receives full Gameplay Effect data, other clients only tags and cues.
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	// PlayerStates replicate slowly by default. GAS state lives here, so match the pawn's rate.
	SetNetUpdateFrequency(100.f);
}

UAbilitySystemComponent* AAbilityForgePlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
