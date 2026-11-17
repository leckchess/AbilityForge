#include "Core/AbilityForgeGameplayTags.h"

namespace AbilityForgeTags
{
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Dash, "Ability.Dash", "Short forward dash.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Dashing, "State.Dashing", "Owned while the Dash ability is active.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Stunned, "State.Stunned", "Blocks abilities while present.");
}
