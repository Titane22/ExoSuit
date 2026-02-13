// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/Characters/Player_Base.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Components/TimelineComponent.h"
#include "AbilitySystemComponent.h"
#include "Gameplay/GAS/EXOGameplayTags.h"

APlayer_Base::APlayer_Base()
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 250.0f;
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->SocketOffset = FVector(0.0f, 60.0f, 20.0f);
	CameraBoom->bEnableCameraLag = true;
	CameraBoom->CameraLagSpeed = 12.0f;
	CameraBoom->bEnableCameraRotationLag = true;
	CameraBoom->CameraRotationLagSpeed = 15.0f;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	CameraTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("CameraTimeline"));
}

void APlayer_Base::BeginPlay()
{
	Super::BeginPlay();
	DefaultMaxWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
	DefaultMaxFlySpeed = GetCharacterMovement()->MaxFlySpeed;

	if (CameraTransitionCurve && CameraTimeline)
	{
		FOnTimelineFloat UpdateCallback;
		UpdateCallback.BindUFunction(this, FName("OnCameraTimelineUpdate"));
		CameraTimeline->AddInterpFloat(CameraTransitionCurve, UpdateCallback);
		CameraTimeline->SetLooping(false);
	}

	if (const APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void APlayer_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Camera offset: shoulder swap + afterburner center blend
	const float ShoulderY = FMath::Abs(CameraOffset.Y) * ShoulderDirection;
	const float TargetY = FMath::Lerp(ShoulderY, 0.0f, CameraTimelineAlpha);
	FVector CurrentOffset = CameraBoom->SocketOffset;
	CurrentOffset.Y = FMath::FInterpTo(CurrentOffset.Y, TargetY, DeltaTime, ShoulderSwapSpeed);
	CameraBoom->SocketOffset = CurrentOffset;

	// Afterburner: smoothed look + auto-forward
	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	if (ASC && ASC->HasMatchingGameplayTag(EXOTags::State_Movement_Afterburner) && Controller)
	{
		SmoothedAfterburnerLook = FMath::Vector2DInterpTo(SmoothedAfterburnerLook, AfterburnerLookInput, DeltaTime, AfterburnerLookLagSpeed);
		AddControllerYawInput(SmoothedAfterburnerLook.X);
		AddControllerPitchInput(SmoothedAfterburnerLook.Y);
		AfterburnerLookInput = FVector2D::ZeroVector;

		const FRotator ControlRotation = Controller->GetControlRotation();
		const FVector ForwardDir = FRotationMatrix(ControlRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(ForwardDir, 1.0f);
	}
	else
	{
		SmoothedAfterburnerLook = FVector2D::ZeroVector;
	}
}

void APlayer_Base::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayer_Base::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayer_Base::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		EnhancedInputComponent->BindAction(ShiftAction, ETriggerEvent::Started, this, &APlayer_Base::ShiftPressed);
		EnhancedInputComponent->BindAction(ShiftAction, ETriggerEvent::Triggered, this, &APlayer_Base::Shift);
		EnhancedInputComponent->BindAction(ShiftAction, ETriggerEvent::Completed, this, &APlayer_Base::StopShift);

		EnhancedInputComponent->BindAction(ShiftCameraAction, ETriggerEvent::Started, this, &APlayer_Base::ShiftCamera);

		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Started, this, &APlayer_Base::Dash);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &APlayer_Base::StopMove);
	}
}

void APlayer_Base::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();
	CachedMoveInput = MovementVector;
	if (!Controller) return;

	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	if (ASC && ASC->HasMatchingGameplayTag(EXOTags::State_Movement_Afterburner))
	{
		const FRotator ControlRotation = Controller->GetControlRotation();
		const FVector RightDir = FRotationMatrix(ControlRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(FVector::UpVector, MovementVector.Y);
		AddMovementInput(RightDir, MovementVector.X);
	}
	else
	{
		const FRotator YawRotation(0, Controller->GetControlRotation().Yaw, 0);
		const FVector ForwardDir = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDir = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(ForwardDir, MovementVector.Y);
		AddMovementInput(RightDir, MovementVector.X);
	}
}

void APlayer_Base::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();
	if (!Controller) return;

	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	if (ASC && ASC->HasMatchingGameplayTag(EXOTags::State_Movement_Afterburner))
	{
		AfterburnerLookInput = LookAxisVector * AfterburnerLookSensitivityMultiplier;
	}
	else
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void APlayer_Base::ShiftPressed()
{
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		FGameplayTagContainer AfterburnerTag;
		AfterburnerTag.AddTag(EXOTags::State_Movement_Afterburner);
		ASC->TryActivateAbilitiesByTag(AfterburnerTag);
	}
}

void APlayer_Base::Shift()
{
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		if (!GetCharacterMovement()->IsFalling() && !GetCharacterMovement()->IsFlying())
		{
			FGameplayTagContainer SprintTag;
			SprintTag.AddTag(EXOTags::State_Movement_Sprinting);
			ASC->TryActivateAbilitiesByTag(SprintTag);
		}
	}
}

void APlayer_Base::StopShift()
{
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		FGameplayTagContainer GameplayTags;
		GameplayTags.AddTag(EXOTags::State_Movement_Sprinting);
		GameplayTags.AddTag(EXOTags::State_Movement_Afterburner);
		ASC->CancelAbilities(&GameplayTags);
	}
}

void APlayer_Base::ShiftCamera()
{
	ShoulderDirection *= -1.0f;
}

void APlayer_Base::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	if (ASC && ASC->HasMatchingGameplayTag(EXOTags::State_Movement_AfterburnerLanding))
	{
		if (AfterburnerLandingMontage)
		{
			if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
			{
				GetCharacterMovement()->DisableMovement();
				AnimInstance->Montage_Play(AfterburnerLandingMontage);

				FOnMontageEnded EndDelegate;
				EndDelegate.BindUObject(this, &APlayer_Base::OnLandingMontageEnded);
				AnimInstance->Montage_SetEndDelegate(EndDelegate, AfterburnerLandingMontage);
				return;
			}
		}
		ASC->RemoveLooseGameplayTag(EXOTags::State_Movement_AfterburnerLanding);
	}
}

void APlayer_Base::OnCameraTimelineUpdate(float Value)
{
	CameraTimelineAlpha = Value;
}

void APlayer_Base::OnLandingMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		ASC->RemoveLooseGameplayTag(EXOTags::State_Movement_AfterburnerLanding);
	}
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
}

void APlayer_Base::StopMove()
{
	CachedMoveInput = FVector2D::ZeroVector;
}

void APlayer_Base::Dash()
{
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		FGameplayTagContainer DashTag;
		DashTag.AddTag(EXOTags::State_Movement_Dashing);
		ASC->TryActivateAbilitiesByTag(DashTag);
	}
}

void APlayer_Base::PlayCameraTimeline(bool bForward)
{
	if (CameraTimeline)
	{
		if (bForward)
		{
			CameraTimeline->Play();
		}
		else
		{
			CameraTimeline->Reverse();
		}
	}
}
