#pragma once

#include "NativeGameplayTags.h"

namespace EXOTags
{
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Movement_Sprinting);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Movement_Afterburner);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Movement_AfterburnerLanding);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Movement_Dashing);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Buff_Invincible);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Cooldown_Dash);

	// Message channels
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Dash_Duration_Message);
}
