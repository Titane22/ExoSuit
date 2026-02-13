// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/GAS/AttributeSet/HealthAttributeSet.h"

UHealthAttributeSet::UHealthAttributeSet():
	MaxHealth(100.f), MaxShield(100.f)
{
	InitHealth(GetMaxHealth());
	InitShield(GetMaxShield());
}

void UHealthAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	if (Attribute == GetMaxHealthAttribute())
	{
		NewValue = FMath::Max<float>(1.f, NewValue);
	}
	else if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp<float>(NewValue, 0.f, GetMaxHealth());
	}
	else if (Attribute == GetMaxShieldAttribute())
	{
		NewValue = FMath::Max<float>(1.f, NewValue);
	}
	else if (Attribute == GetShieldAttribute())
	{
		NewValue = FMath::Clamp<float>(NewValue, 0.f, GetMaxShield());
	}
}

void UHealthAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	if (Attribute == GetMaxHealthAttribute())
	{
		OnMaxHealthChanged.Broadcast(OldValue, NewValue);
	}
	else if (Attribute == GetHealthAttribute())
	{
		OnHealthChanged.Broadcast(OldValue, NewValue);
	}
	else if (Attribute == GetMaxShieldAttribute())
	{
		OnMaxShieldChanged.Broadcast(OldValue, NewValue);
	}
	else if (Attribute == GetShieldAttribute())
	{
		OnShieldChanged.Broadcast(OldValue, NewValue);
	}
}
