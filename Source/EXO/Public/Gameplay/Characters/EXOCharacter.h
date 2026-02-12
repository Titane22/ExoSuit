// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "EXOCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UEXOAbilitySystemComponent;
class UGameplayAbility;
struct FInputActionValue;

UCLASS()
class EXO_API AEXOCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AEXOCharacter();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;

public:
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	UEXOAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS")
	TArray<TSubclassOf<UGameplayAbility>> DefaultAbilities;

private:
	bool bAbilitiesGranted = false;
};
