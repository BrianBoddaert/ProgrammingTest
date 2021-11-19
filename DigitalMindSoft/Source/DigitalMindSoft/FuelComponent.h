// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "FuelComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DIGITALMINDSOFT_API UFuelComponent : public UStaticMeshComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFuelComponent();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void UpdateFuel(float deltaTime, float carSpeedKMH, float engineRotations, float maxEngineRotations);



protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// In liters
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float m_MaxFuelCapacity = 1.0f; // would be const if Uproperty allowed it

	// in L/km, can go up to 2 times this at full throttle
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float m_BaseFuelConsumptionRate = 1.0f; //would be const if Uproperty allowed it

	// In liters
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float m_CurrentFuel; 

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	//	float m_CurrentEngineRotations;

	const static int32 m_NumCurveSamples = 100; //would be const if Uproperty allowed it

	float m_FuelConsumptionCurvePoints[m_NumCurveSamples];

};
