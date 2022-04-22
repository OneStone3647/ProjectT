// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectTGameMode.h"
#include "PTPlayerCharacter.h"
#include "UObject/ConstructorHelpers.h"

AProjectTGameMode::AProjectTGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/BP_PTPlayerCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
