// Copyright Epic Games, Inc. All Rights Reserved.

#include "DigitalMindSoftGameMode.h"
#include "FuelCar.h"
#include "FuelHud.h"

ADigitalMindSoftGameMode::ADigitalMindSoftGameMode()
{
	DefaultPawnClass = AFuelCar::StaticClass();
	HUDClass = AFuelHud::StaticClass();
}
