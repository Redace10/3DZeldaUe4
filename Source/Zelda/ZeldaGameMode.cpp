// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "ZeldaGameMode.h"
#include "ZeldaCharacter.h"
#include "UObject/ConstructorHelpers.h"

AZeldaGameMode::AZeldaGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
    Rupees = 0;
}

uint8 AZeldaGameMode::GetRupees() const
{
    return Rupees;
}
