// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Characters/EXOCharacter.h"
class UAnimMontage;
class UTimelineComponent;
class UCurveFloat;

#include "Player_Base.generated.h"

/**
 *
 */
UCLASS()
class EXO_API APlayer_Base : public AEXOCharacter
{
	GENERATED_BODY()
public:
	APlayer_Base();

	// Public accessors for GA classes
	void PlayCameraTimeline(bool bForward);
	FVector2D GetCachedMoveInput() const { return CachedMoveInput; }
	float GetDefaultMaxWalkSpeed() const { return DefaultMaxWalkSpeed; }

	void StartSprintCamera();
	void StopSprintCamera();

	// Movement parameters (public for GA access)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|Sprint")
	float SprintSpeedMultiplier = 1.3f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|Afterburner")
	float AfterburnerSpeedMultiplier = 1.8f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|Afterburner")
	float AfterburnerLookSensitivityMultiplier = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|Afterburner")
	UAnimMontage* AfterburnerLandingMontage;

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void Landed(const FHitResult& Hit) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* ShiftAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* ShiftCameraAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* DashAction;

	// Camera Parameters
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	FVector CameraOffset = FVector(0.0f, 60.0f, 20.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float ShoulderSwapSpeed = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	UCurveFloat* CameraTransitionCurve;

	UPROPERTY()
	UTimelineComponent* CameraTimeline;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Afterburner")
	float AfterburnerLookLagSpeed = 6.0f;

	// Sprint camera parameters
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Sprint")
	float SprintCameraArmLengthOffset = 150.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Sprint")
	float SprintCameraInterpSpeed = 3.0f;

	float DefaultArmLength;
	float DefaultCameraLagSpeed;
	float SprintTargetArmLength;
	bool bSprintCameraWaiting = false;
	bool bSprintCameraReturning = false;

	FVector2D AfterburnerLookInput;
	FVector2D SmoothedAfterburnerLook;
	float CameraTimelineAlpha = 0.0f;
	float ShoulderDirection = 1.0f;

	UFUNCTION()
	void OnCameraTimelineUpdate(float Value);

	// Cached defaults (saved in BeginPlay)
	float DefaultMaxWalkSpeed;
	float DefaultMaxFlySpeed;

	// Cached input
	FVector2D CachedMoveInput;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void ShiftPressed();
	void Shift();
	void StopShift();
	void ShiftCamera();
	void Dash();
	void StopMove();
	virtual void Jump() override;
	virtual void StopJumping() override;
	
	void OnLandingMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	
public:
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
};
