// Fill out your copyright notice in the Description page of Project Settings.


#include "FuelComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInterface.h"

//const int32 UFuelComponent::m_NumCurveSamples = 100; //would be const if Uproperty allowed it

// Sets default values for this component's properties
UFuelComponent::UFuelComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> baseFuelCompMesh(TEXT("/Game/Geometry/Meshes/1M_Cube"));
	if (baseFuelCompMesh.Succeeded())
		SetStaticMesh(baseFuelCompMesh.Object);

	static ConstructorHelpers::FObjectFinder<UMaterial> baseFuelCompMaterial(TEXT("/Game/VehicleAdv/Materials/M_FuelTank"));
	if (baseFuelCompMaterial.Succeeded())
		m_pMaterial = baseFuelCompMaterial.Object;

	SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	InitializeFuelConsumptionCurve();
}

// Called when the game starts
void UFuelComponent::BeginPlay()
{
	Super::BeginPlay();
	SetRelativeLocation(FVector(-30, 0, 40));
	float baseScale = 0.2f;
	SetWorldScale3D(FVector(baseScale, baseScale, baseScale));
	m_pMaterialInstanceDynamic = UMaterialInstanceDynamic::Create(m_pMaterial, NULL);
	SetMaterial(0, m_pMaterialInstanceDynamic);
	m_BaseScaleY = baseScale;
	SetVisibility(m_Visible);
	m_CurrentFuel = m_MaxFuelCapacity;
}

void UFuelComponent::InitializeFuelConsumptionCurve()
{
	FVector bezierPoints[4];
	bezierPoints[0] = FVector(1, 1, 0);
	bezierPoints[1] = FVector(1.5, 1, 0);
	bezierPoints[2] = FVector(1, 1, 0);
	bezierPoints[3] = FVector(2, 2, 0);

	FOccluderVertexArray fuelConsumptionPoints;

	// Fuel consumption increase rate dependant on engine rotations using a bezier curve
	FVector::EvaluateBezier(bezierPoints, m_NumCurveSamples, fuelConsumptionPoints);

	// Only need one dimension.
	for (size_t i = 0; i < m_NumCurveSamples; i++)
		m_FuelConsumptionCurvePoints[i] = fuelConsumptionPoints[i].Y;
}

void UFuelComponent::UpdateFuel(float deltaTime, float carSpeedKMH, float engineRotations, float maxEngineRotations)
{
	// Converts to KM/S
	float carSpeedKMS = carSpeedKMH / 3600;
	// Considering deltaTime to see how many kilometers the player travelled in the last frame
	float carSpeedKMDS = carSpeedKMS * deltaTime;
	// puts engine rotations on a scale of 0 to 1 dependant on the max 
	float engineRotationsFraction = engineRotations / maxEngineRotations;
	// scale from 0 to 1 turns into 0 to 99 index to access bezier curve point
	int throttleCurvePoint = engineRotationsFraction * (m_NumCurveSamples - 1);
	// Uses a compile time set bezier curve to indicate current fuelConsumption increase.
	m_CurrentFuelConsumptionRate = m_BaseFuelConsumptionRate * m_FuelConsumptionCurvePoints[throttleCurvePoint];

	m_CurrentFuel -= m_CurrentFuelConsumptionRate * carSpeedKMDS;

	if (m_CurrentFuel < 0)
		m_CurrentFuel = 0;
}

// Called every frame
void UFuelComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	ChangeVisualAspectDependingOnFuel();
	// ...
}

void UFuelComponent::ChangeVisualAspectDependingOnFuel()
{
	m_Visible = GetVisibleFlag();

	if (!m_Visible || !m_pMaterialInstanceDynamic)
		return;

	// 0 to 1 how much fuel there is left compared to the max
	float fuelFraction = m_CurrentFuel / m_MaxFuelCapacity;

	// Set the color red dependant on the current fuel
	FLinearColor color{ 1 - fuelFraction,0,0 };
	m_pMaterialInstanceDynamic->SetVectorParameterValue(TEXT("MainColor"), color);

	// Rescale the box's Z axis dependant on the current fuel
	FVector newScale{};
	newScale = GetWorldScale3D();
	newScale.Z = m_BaseScaleY * (fuelFraction);
	if (newScale.Z > 0)
		SetWorldScale3D(newScale);


}


// Function that was not in USceneComponent for some reason
FVector UFuelComponent::GetWorldScale3D() const
{

	// If attached to something, transform into local space
	if (GetAttachParent() != nullptr && !IsUsingAbsoluteScale())
	{
		FTransform ParentToWorld = GetAttachParent()->GetSocketTransform(GetAttachSocketName());
		return GetRelativeScale3D() / ParentToWorld.GetSafeScaleReciprocal(ParentToWorld.GetScale3D());
	}

	return GetRelativeScale3D();
}