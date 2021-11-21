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
}

void AFuelCar::Tick(float Delta)
{
	Super::Tick(Delta);
	UWheeledVehicleMovementComponent* pMovementComp = GetVehicleMovement();

	float carSpeedKMH = FMath::Abs(pMovementComp->GetForwardSpeed()) * 0.036f;

	m_pFuelComponent->UpdateFuel(Delta, carSpeedKMH, pMovementComp->GetEngineRotationSpeed(), pMovementComp->GetEngineMaxRotationSpeed());
	
}

void AFuelCar::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	AWheeledVehicle::SetupPlayerInputComponent(PlayerInputComponent);

	// set up gameplay key bindings
	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AFuelCar::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ADigitalMindSoftPawn::MoveRight);
	PlayerInputComponent->BindAxis(LookUpBinding);
	PlayerInputComponent->BindAxis(LookRightBinding);

	PlayerInputComponent->BindAction("Handbrake", IE_Pressed, this, &ADigitalMindSoftPawn::OnHandbrakePressed);
	PlayerInputComponent->BindAction("Handbrake", IE_Released, this, &ADigitalMindSoftPawn::OnHandbrakeReleased);
	PlayerInputComponent->BindAction("SwitchCamera", IE_Pressed, this, &ADigitalMindSoftPawn::OnToggleCamera);

	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ADigitalMindSoftPawn::OnResetVR);
	
	PlayerInputComponent->BindAction("ToggleFuelComponentVisibility", IE_Pressed, this, &AFuelCar::ToggleFuelComponentVisibility);
}

void AFuelCar::MoveForward(float val)
{
	if (m_pFuelComponent && m_pFuelComponent->IsEmpty())
		val = 0.0f;

	GetVehicleMovementComponent()->SetThrottleInput(val);
}

void AFuelCar::ToggleFuelComponentVisibility()
{
	if (m_pFuelComponent)
		m_pFuelComponent->ToggleVisibility();
}

//UFuelComponent* AFuelCar::GetFuelComponent() const;
//{ 
//	return m_pFuelComponent;
//}