#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "EXOGameplayAbility.generated.h"

class APlayer_Base;
class UCharacterMovementComponent;

UCLASS(Abstract)
class EXO_API UEXOGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UEXOGameplayAbility();

protected:
	APlayer_Base* GetPlayerBase() const;
	UCharacterMovementComponent* GetMovementComponent() const;
};
