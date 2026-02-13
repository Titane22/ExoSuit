// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/Characters/EXOCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Gameplay/GAS/EXOAbilitySystemComponent.h"
#include "Gameplay/GAS/AttributeSet/HealthAttributeSet.h"
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

	ASC = CreateDefaultSubobject<UEXOAbilitySystemComponent>(TEXT("Ability System Component"));
	HealthAttributeSet = CreateDefaultSubobject<UHealthAttributeSet>(FName("HealthAttributeSet"));

	ASC->AddAttributeSetSubobject<UHealthAttributeSet>(HealthAttributeSet);
}

UAbilitySystemComponent* AEXOCharacter::GetAbilitySystemComponent() const
{
	return ASC;
}

void AEXOCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AEXOCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	EXOPostInitializeComponents();

	ASC->SetNumericAttributeBase(HealthAttributeSet->GetMaxHealthAttribute(), 200.f);
	ASC->SetNumericAttributeBase(HealthAttributeSet->GetHealthAttribute(), 200.f);
	ASC->SetNumericAttributeBase(HealthAttributeSet->GetMaxShieldAttribute(), 100.f);
	ASC->SetNumericAttributeBase(HealthAttributeSet->GetShieldAttribute(), 100.f);
}

void AEXOCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (ASC)
	{
		ASC->InitAbilityActorInfo(this, this);

		if (!bAbilitiesGranted)
		{
			for (const TSubclassOf<UGameplayAbility>& AbilityClass : DefaultAbilities)
			{
				if (AbilityClass)
				{
					ASC->GiveAbility(FGameplayAbilitySpec(AbilityClass, 1, INDEX_NONE, this));
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
