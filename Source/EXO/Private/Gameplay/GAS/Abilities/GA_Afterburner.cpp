#include "Gameplay/GAS/Abilities/GA_Afterburner.h"
#include "Gameplay/GAS/EXOGameplayTags.h"
#include "Gameplay/Characters/Player_Base.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

UGA_Afterburner::UGA_Afterburner()
{
	FGameplayTagContainer AssetTags;
	AssetTags.AddTag(EXOTags::State_Movement_Afterburner);
	SetAssetTags(AssetTags);
	ActivationOwnedTags.AddTag(EXOTags::State_Movement_Afterburner);

	ActivationBlockedTags.AddTag(EXOTags::State_Movement_AfterburnerLanding);
	ActivationBlockedTags.AddTag(EXOTags::State_Movement_Dashing);

	CancelAbilitiesWithTag.AddTag(EXOTags::State_Movement_Sprinting);
}

bool UGA_Afterburner::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
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

	return MoveComp->IsFalling();
}

void UGA_Afterburner::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	APlayer_Base* Player = GetPlayerBase();
	UCharacterMovementComponent* MoveComp = GetMovementComponent();
	if (!Player || !MoveComp)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	CachedDefaultFlySpeed = MoveComp->MaxFlySpeed;

	MoveComp->SetMovementMode(MOVE_Flying);
	MoveComp->MaxFlySpeed = Player->GetDefaultMaxWalkSpeed() * Player->AfterburnerSpeedMultiplier;
	MoveComp->bOrientRotationToMovement = false;
	MoveComp->bUseControllerDesiredRotation = true;

	Player->PlayCameraTimeline(true);
}

void UGA_Afterburner::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	APlayer_Base* Player = GetPlayerBase();
	UCharacterMovementComponent* MoveComp = GetMovementComponent();

	if (Player && MoveComp)
	{
		MoveComp->SetMovementMode(MOVE_Falling);
		MoveComp->MaxFlySpeed = CachedDefaultFlySpeed;
		MoveComp->bOrientRotationToMovement = true;
		MoveComp->bUseControllerDesiredRotation = false;

		Player->PlayCameraTimeline(false);

		if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
		{
			ASC->AddLooseGameplayTag(EXOTags::State_Movement_AfterburnerLanding);
		}
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
