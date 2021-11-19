// Copyright Epic Games, Inc. All Rights Reserved.

#include "DigitalMindSoftGameMode.h"
#include "FuelCar.h"
#include "DigitalMindSoftHud.h"

ADigitalMindSoftGameMode::ADigitalMindSoftGameMode()
{
	DefaultPawnClass = AFuelCar::StaticClass();
	HUDClass = ADigitalMindSoftHud::StaticClass();
}
