// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "TacticalShooterAIGameMode.h"
#include "TacticalShooterAIHUD.h"
#include "TacticalShooterAICharacter.h"
#include "UObject/ConstructorHelpers.h"

ATacticalShooterAIGameMode::ATacticalShooterAIGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ATacticalShooterAIHUD::StaticClass();
}
