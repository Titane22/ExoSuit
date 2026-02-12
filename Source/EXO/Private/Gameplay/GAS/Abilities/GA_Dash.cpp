#include "Gameplay/GAS/Abilities/GA_Dash.h"
#include "Gameplay/GAS/EXOGameplayTags.h"
#include "Gameplay/Characters/Player_Base.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "GameFramework/CharacterMovementComponent.h"

UGA_Dash::UGA_Dash()
{
	FGameplayTagContainer AssetTags;
	AssetTags.AddTag(EXOTags::State_Movement_Dashing);
	SetAssetTags(AssetTags);
	ActivationOwnedTags.AddTag(EXOTags::State_Movement_Dashing);
	ActivationOwnedTags.AddTag(EXOTags::State_Buff_Invincible);

	ActivationBlockedTags.AddTag(EXOTags::State_Movement_Dashing);
	ActivationBlockedTags.AddTag(EXOTags::State_Cooldown_Dash);
	ActivationBlockedTags.AddTag(EXOTags::State_Movement_AfterburnerLanding);
	ActivationBlockedTags.AddTag(EXOTags::State_Movement_Afterburner);
}

void UGA_Dash::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	APlayer_Base* Player = GetPlayerBase();
	UCharacterMovementComponent* MoveComp = GetMovementComponent();
	if (!Player || !MoveComp || !Player->GetController())
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	const FVector2D MoveInput = Player->GetCachedMoveInput();
	UAnimMontage* SelectedMontage = nullptr;
	FVector LaunchDir = FVector::ZeroVector;
	const FRotator YawRotation(0, Player->GetController()->GetControlRotation().Yaw, 0);

	if (FMath::Abs(MoveInput.X) > FMath::Abs(MoveInput.Y))
	{
		if (MoveInput.X > 0)
		{
			SelectedMontage = Player->DashRightMontage;
			LaunchDir = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		}
		else
		{
			SelectedMontage = Player->DashLeftMontage;
			LaunchDir = -FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		}
	}
	else
	{
		if (MoveInput.Y < 0)
		{
			SelectedMontage = Player->DashBackwardMontage;
			LaunchDir = -FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		}
		else
		{
			SelectedMontage = Player->DashForwardMontage;
			LaunchDir = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		}
	}

	if (!SelectedMontage)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	Player->LaunchCharacter(LaunchDir * Player->DashLaunchForce, true, true);
	MoveComp->DisableMovement();

	UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this, NAME_None, SelectedMontage, 1.0f);
	MontageTask->OnCompleted.AddDynamic(this, &UGA_Dash::OnMontageCompleted);
	MontageTask->OnBlendOut.AddDynamic(this, &UGA_Dash::OnMontageCompleted);
	MontageTask->OnInterrupted.AddDynamic(this, &UGA_Dash::OnMontageCancelled);
	MontageTask->OnCancelled.AddDynamic(this, &UGA_Dash::OnMontageCancelled);
	MontageTask->ReadyForActivation();
}

void UGA_Dash::OnMontageCompleted()
{
	if (UCharacterMovementComponent* MoveComp = GetMovementComponent())
	{
		MoveComp->SetMovementMode(MOVE_Walking);
	}

	ApplyDashCooldown();
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
}

void UGA_Dash::OnMontageCancelled()
{
	if (UCharacterMovementComponent* MoveComp = GetMovementComponent())
	{
		MoveComp->SetMovementMode(MOVE_Walking);
	}

	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, true);
}

void UGA_Dash::ApplyDashCooldown()
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	APlayer_Base* Player = GetPlayerBase();
	if (!ASC || !Player)
	{
		return;
	}

	ASC->AddLooseGameplayTag(EXOTags::State_Cooldown_Dash);

	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(
			CooldownTimerHandle,
			FTimerDelegate::CreateWeakLambda(ASC, [ASC]()
			{
				ASC->RemoveLooseGameplayTag(EXOTags::State_Cooldown_Dash);
			}),
			Player->DashCooldownDuration,
			false
		);
	}
}

void UGA_Dash::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
