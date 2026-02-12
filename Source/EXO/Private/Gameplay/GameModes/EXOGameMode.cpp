// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/GameModes/EXOGameMode.h"
#include "Gameplay/Characters/EXOCharacter.h"

AEXOGameMode::AEXOGameMode()
{
	DefaultPawnClass = AEXOCharacter::StaticClass();
}
