#include "Gameplay/GAS/EXOGameplayAbility.h"
#include "Gameplay/Characters/Player_Base.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Gameplay/GAS/EXOAbilitySystemComponent.h"

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

AController* UEXOGameplayAbility::GetControllerFromActorInfo() const
{
	if (CurrentActorInfo)
	{
		// ActorInfo에 저장된 PlayerController를 우선 반환합니다.
		if (AController* PC = CurrentActorInfo->PlayerController.Get())
		{
			return PC;
		}

		// 만약 ActorInfo에 Controller가 없다면 AvatarActor(Pawn)로부터 직접 가져옵니다.
		if (AActor* AvatarActor = CurrentActorInfo->AvatarActor.Get())
		{
			if (APawn* Pawn = Cast<APawn>(AvatarActor))
			{
				return Pawn->GetController();
			}
		}
	}
	return nullptr;
}

AEXOCharacter* UEXOGameplayAbility::GetExoCharacter() const
{
	// EXOCharacter가 PlayerBase를 상속받았거나 동일한 구조라면 아래와 같이 캐스팅합니다.
	return Cast<AEXOCharacter>(GetAvatarActorFromActorInfo());
}

UEXOAbilitySystemComponent* UEXOGameplayAbility::GetExoAbilitySystemComponent() const
{
	if (CurrentActorInfo)
	{
		// Lyra 스타일: ActorInfo에 캐싱된 AbilitySystemComponent를 우리 클래스로 캐스팅
		return Cast<UEXOAbilitySystemComponent>(CurrentActorInfo->AbilitySystemComponent.Get());
	}
	return nullptr;
}

UCameraComponent* UEXOGameplayAbility::GetCamera() const
{
	if (!GetPlayerBase())
		return nullptr;
	
	return GetPlayerBase()->GetFollowCamera();
}
