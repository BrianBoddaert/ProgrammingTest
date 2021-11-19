// Fill out your copyright notice in the Description page of Project Settings.


#include "FuelCar.h"
#include "WheeledVehicle.h"
#include "WheeledVehicleMovementComponent.h"
#include "FuelComponent.h"

AFuelCar::AFuelCar()
	:ADigitalMindSoftPawn{}
{
	m_pFuelComponent = CreateDefaultSubobject<UFuelComponent>(TEXT("FuelComponent"));
	m_pFuelComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	m_pFuelComponent->SetRelativeLocation(FVector(-30, 0, 40));
	m_pFuelComponent->SetWorldScale3D(FVector(0.4f, 0.4f, 0.4f));
}

void AFuelCar::Tick(float Delta)
{
	Super::Tick(Delta);
	UWheeledVehicleMovementComponent* pMovementComp = GetVehicleMovement();

	float carSpeedKMH = FMath::Abs(pMovementComp->GetForwardSpeed()) * 0.036f;

	//float throttleInput = GetVehicleMovementComponent()->GetThrottleInput();
	m_pFuelComponent->UpdateFuel(Delta, carSpeedKMH, pMovementComp->GetEngineRotationSpeed(), pMovementComp->GetEngineMaxRotationSpeed());
	
}

void AFuelCar::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// set up gameplay key bindings
	check(PlayerInputComponent);
	
}

