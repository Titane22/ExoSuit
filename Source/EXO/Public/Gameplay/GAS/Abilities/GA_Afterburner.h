#pragma once

#include "CoreMinimal.h"
#include "Gameplay/GAS/EXOGameplayAbility.h"
#include "GA_Afterburner.generated.h"

UCLASS()
class EXO_API UGA_Afterburner : public UEXOGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_Afterburner();

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

private:
	float CachedDefaultFlySpeed = 0.0f;
};
