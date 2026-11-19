#include "Core/AbilityForgePlayerState.h"

#include "AbilitySystemComponent.h"
#include "Attributes/AbilityForgeAttributeSet.h"

AAbilityForgePlayerState::AAbilityForgePlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);

	// Mixed: the owning client receives full Gameplay Effect data, other clients only tags and cues.
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	// Attribute Sets created as subobjects of the ASC's owner are found and registered by the ASC.
	AttributeSet = CreateDefaultSubobject<UAbilityForgeAttributeSet>(TEXT("AttributeSet"));

	// PlayerStates replicate slowly by default. GAS state lives here, so match the pawn's rate.
	SetNetUpdateFrequency(100.f);
}

UAbilitySystemComponent* AAbilityForgePlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
