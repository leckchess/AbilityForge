#pragma once

#include "NativeGameplayTags.h"

/** Native Gameplay Tags. Declared in C++ so code can use them without string lookups. */
namespace AbilityForgeTags
{
	// Ability identity
	ABILITYFORGE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Dash);

	// States granted while an ability or effect is active
	ABILITYFORGE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Dashing);
	ABILITYFORGE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Stunned);

	// Cooldowns
	ABILITYFORGE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Cooldown_Dash);

	// SetByCaller magnitudes
	ABILITYFORGE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Data_Damage);
}
