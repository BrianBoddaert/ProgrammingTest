// Copyright Epic Games, Inc. All Rights Reserved.

#include "DigitalMindSoftGameMode.h"
#include "DigitalMindSoftPawn.h"
#include "FuelHud.h"

ADigitalMindSoftGameMode::ADigitalMindSoftGameMode()
{
	DefaultPawnClass = ADigitalMindSoftPawn::StaticClass();
	HUDClass = AFuelHud::StaticClass();
}
