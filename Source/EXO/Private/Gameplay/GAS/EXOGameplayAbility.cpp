#include "Gameplay/GAS/EXOGameplayAbility.h"
#include "Gameplay/Characters/Player_Base.h"
#include "GameFramework/CharacterMovementComponent.h"

UEXOGameplayAbility::UEXOGameplayAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

APlayer_Base* UEXOGameplayAbility::GetPlayerBase() const
{
	return Cast<APlayer_Base>(GetAvatarActorFromActorInfo());
}

UCharacterMovementComponent* UEXOGameplayAbility::GetMovementComponent() const
{
	if (APlayer_Base* Player = GetPlayerBase())
	{
		return Player->GetCharacterMovement();
	}
	return nullptr;
}
