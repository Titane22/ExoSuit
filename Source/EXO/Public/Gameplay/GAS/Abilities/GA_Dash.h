#pragma once

#include "CoreMinimal.h"
#include "Gameplay/GAS/EXOGameplayAbility.h"
#include "GA_Dash.generated.h"

UCLASS()
class EXO_API UGA_Dash : public UEXOGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_Dash();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

private:
	UFUNCTION()
	void OnMontageCompleted();

	UFUNCTION()
	void OnMontageCancelled();

	void ApplyDashCooldown();

	FTimerHandle CooldownTimerHandle;
};
