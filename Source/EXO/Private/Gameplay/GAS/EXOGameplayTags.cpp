#include "Gameplay/GAS/EXOGameplayTags.h"

namespace EXOTags
{
	UE_DEFINE_GAMEPLAY_TAG(State_Movement_Sprinting, "State.Movement.Sprinting");
	UE_DEFINE_GAMEPLAY_TAG(State_Movement_Jumping, "State.Movement.Jumping");
	UE_DEFINE_GAMEPLAY_TAG(State_Movement_Afterburner, "State.Movement.Afterburner");
	UE_DEFINE_GAMEPLAY_TAG(State_Movement_AfterburnerLanding, "State.Movement.AfterburnerLanding");
	UE_DEFINE_GAMEPLAY_TAG(State_Movement_Dashing, "State.Movement.Dashing");
	UE_DEFINE_GAMEPLAY_TAG(State_Buff_Invincible, "State.Buff.Invincible");
	UE_DEFINE_GAMEPLAY_TAG(State_Cooldown_Dash, "State.Cooldown.Dash");

	// Message channels
	UE_DEFINE_GAMEPLAY_TAG(Ability_Dash_Duration_Message, "Ability.Dash.Duration.Message");
}
