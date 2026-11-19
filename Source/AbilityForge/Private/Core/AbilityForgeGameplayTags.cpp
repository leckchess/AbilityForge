#include "Core/AbilityForgeGameplayTags.h"

namespace AbilityForgeTags
{
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Dash, "Ability.Dash", "Short forward dash.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Melee, "Ability.Melee", "Montage-driven melee attack.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Sprint, "Ability.Sprint", "Assignment 1: sprint while held.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Dashing, "State.Dashing", "Owned while the Dash ability is active.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Stunned, "State.Stunned", "Blocks abilities while present.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Attacking, "State.Attacking", "Owned while the melee attack is active.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Sprinting, "State.Sprinting", "Owned while the Sprint ability is active.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Cooldown_Dash, "Cooldown.Dash", "Granted by the Dash cooldown effect.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Data_Damage, "Data.Damage", "SetByCaller magnitude for damage effects.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Event_Melee_Hit, "Event.Melee.Hit", "Sent by the attack montage's anim notify at the impact frame.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayCue_Hit, "GameplayCue.Hit", "Hit feedback played on the target.");
}
