// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/Characters/EXOCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Gameplay/GAS/EXOAbilitySystemComponent.h"
#include "Abilities/GameplayAbility.h"

AEXOCharacter::AEXOCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.0f;
	GetCharacterMovement()->JumpZVelocity = 700.0f;
	GetCharacterMovement()->AirControl = 0.35f;

	AbilitySystemComponent = CreateDefaultSubobject<UEXOAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
}

UAbilitySystemComponent* AEXOCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AEXOCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AEXOCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);

		if (!bAbilitiesGranted)
		{
			for (const TSubclassOf<UGameplayAbility>& AbilityClass : DefaultAbilities)
			{
				if (AbilityClass)
				{
					AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(AbilityClass, 1, INDEX_NONE, this));
				}
			}
			bAbilitiesGranted = true;
		}
	}
}

void AEXOCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
