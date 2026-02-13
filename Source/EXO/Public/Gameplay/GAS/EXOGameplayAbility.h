#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Camera/CameraComponent.h"
#include "EXOGameplayAbility.generated.h"

class UEXOAbilitySystemComponent;
class AEXOCharacter;
class APlayer_Base;
class UCharacterMovementComponent;

USTRUCT(BlueprintType)
struct FEXOGAMessage
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<AActor> Instigator = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Duration = 0.f;
};

UCLASS(Abstract)
class EXO_API UEXOGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UEXOGameplayAbility();

protected:
	UFUNCTION(BlueprintCallable, Category = "EXO|Ability")
	APlayer_Base* GetPlayerBase() const;
	
	UFUNCTION(BlueprintCallable, Category = "EXO|Ability")
	UCharacterMovementComponent* GetMovementComponent() const;

	UFUNCTION(BlueprintCallable, Category = "EXO|Ability")
	AController* GetControllerFromActorInfo() const;

	UFUNCTION(BlueprintCallable, Category = "EXO|Ability")
	AEXOCharacter* GetExoCharacter() const;

	UFUNCTION(BlueprintCallable, Category = "EXO|Ability")
	UEXOAbilitySystemComponent* GetExoAbilitySystemComponent() const;

	UFUNCTION(BlueprintCallable, Category = "EXO|Ability")
	UCameraComponent* GetCamera() const;
};
