// Copyright Epic Games, Inc. All Rights Reserved.

#include "DigitalMindSoftGameMode.h"
#include "DigitalMindSoftPawn.h"
#include "DigitalMindSoftHud.h"

ADigitalMindSoftGameMode::ADigitalMindSoftGameMode()
{
	DefaultPawnClass = ADigitalMindSoftPawn::StaticClass();
	HUDClass = ADigitalMindSoftHud::StaticClass();
}
