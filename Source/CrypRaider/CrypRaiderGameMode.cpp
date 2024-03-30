// Copyright Epic Games, Inc. All Rights Reserved.

#include "CrypRaiderGameMode.h"
#include "CrypRaiderCharacter.h"
#include "UObject/ConstructorHelpers.h"

ACrypRaiderGameMode::ACrypRaiderGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
