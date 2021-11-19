// Fill out your copyright notice in the Description page of Project Settings.


#include "FuelComponent.h"

//const int32 UFuelComponent::m_NumCurveSamples = 100; //would be const if Uproperty allowed it

// Sets default values for this component's properties
UFuelComponent::UFuelComponent()
	:m_CurrentFuel{ m_MaxFuelCapacity }
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> baseFuelCompMesh(TEXT("/Game/Geometry/Meshes/1M_Cube"));
	SetStaticMesh(baseFuelCompMesh.Object);

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> baseFuelCompMaterial(TEXT("/Game/VehicleAdv/Materials/MI_FuelTank"));
	SetMaterial(0,baseFuelCompMaterial.Object);

	SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);


	FVector bezierPoints[4];
	bezierPoints[0] = FVector(0, 0, 0);
	bezierPoints[1] = FVector(.5, 0, 0);
	bezierPoints[2] = FVector(0, 0, 0);
	bezierPoints[3] = FVector(1, 1, 0);

	FOccluderVertexArray fuelConsumptionPoints;
	FVector::EvaluateBezier(bezierPoints, m_NumCurveSamples, fuelConsumptionPoints);

	for (size_t i = 0; i < m_NumCurveSamples; i++)
		m_FuelConsumptionCurvePoints[i] = fuelConsumptionPoints[i].Y;
}


// Called when the game starts
void UFuelComponent::BeginPlay()
{
	Super::BeginPlay();
	// ...
	
}


void UFuelComponent::UpdateFuel(float deltaTime, float carSpeedKMH, float engineRotations, float maxEngineRotations)
{
	//m_CurrentFuelConsumptionRate;


	float carSpeedKMS = carSpeedKMH / 3600; // Converts to KM/S
	float carSpeedKMDS = carSpeedKMS * deltaTime; // Considering deltaTime to see how many kilometers the player travelled in the last frame
	
	float engineRotationsPercentage = engineRotations / maxEngineRotations; // puts engine rotations on a scale of 0 to 1 dependant on the max 
	int throttleCurvePoint = engineRotationsPercentage * (m_NumCurveSamples - 1); // scale from 0 to 1 turns into 0 to 99 index to access bezier curve point
	

	float consumptionRate = m_BaseFuelConsumptionRate + m_FuelConsumptionCurvePoints[throttleCurvePoint]; // Grows using a compile time set bezier curve.

	m_CurrentFuel -= consumptionRate * carSpeedKMDS;


	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue,   "Fuel:" + FString::SanitizeFloat(m_CurrentFuel));
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Purple, "EngineRotationsPerSecond:" + FString::SanitizeFloat(engineRotationsPercentage));
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green,  "consumptionRate:" + FString::SanitizeFloat(consumptionRate));
	}
		

	
}

// Called every frame
void UFuelComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

