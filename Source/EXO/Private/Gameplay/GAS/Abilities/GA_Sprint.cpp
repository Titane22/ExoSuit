#include "Gameplay/GAS/Abilities/GA_Sprint.h"
#include "Gameplay/GAS/EXOGameplayTags.h"
#include "Gameplay/Characters/Player_Base.h"
#include "GameFramework/CharacterMovementComponent.h"

UGA_Sprint::UGA_Sprint()
{
	FGameplayTagContainer AssetTags;
	AssetTags.AddTag(EXOTags::State_Movement_Sprinting);
	SetAssetTags(AssetTags);
	ActivationOwnedTags.AddTag(EXOTags::State_Movement_Sprinting);

	ActivationBlockedTags.AddTag(EXOTags::State_Movement_Dashing);
	ActivationBlockedTags.AddTag(EXOTags::State_Movement_AfterburnerLanding);
}

bool UGA_Sprint::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	const UCharacterMovementComponent* MoveComp = nullptr;
	if (const ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get()))
	{
		MoveComp = Character->GetCharacterMovement();
	}

	if (!MoveComp)
	{
		return false;
	}

	return !MoveComp->IsFalling() && !MoveComp->IsFlying();
}

void UGA_Sprint::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	APlayer_Base* Player = GetPlayerBase();
	UCharacterMovementComponent* MoveComp = GetMovementComponent();
	if (!Player || !MoveComp)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	CachedDefaultSpeed = Player->GetDefaultMaxWalkSpeed();
	MoveComp->MaxWalkSpeed = CachedDefaultSpeed * Player->SprintSpeedMultiplier;

	Player->StartSprintCamera();
}

void UGA_Sprint::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (UCharacterMovementComponent* MoveComp = GetMovementComponent())
	{
		if (APlayer_Base* Player = GetPlayerBase())
		{
			MoveComp->MaxWalkSpeed = Player->GetDefaultMaxWalkSpeed();
			Player->StopSprintCamera();
		}
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
