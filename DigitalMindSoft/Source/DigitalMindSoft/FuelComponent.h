// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "FuelComponent.generated.h"

class UMaterialInstanceDynamic;

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

	UFUNCTION(BlueprintCallable)
		inline bool IsEmpty() const {return m_CurrentFuel <= 0;	}

	UFUNCTION(BlueprintCallable)
		inline float GetCurrentFuel() const { return m_CurrentFuel; }

	UFUNCTION(BlueprintCallable)
		inline float GetMaxFuel() const { return m_MaxFuelCapacity; }

	UFUNCTION(BlueprintCallable)
		inline float GetCurrentConsumption() const { return m_CurrentFuelConsumptionRate; }




protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void InitializeFuelConsumptionCurve();
	void ChangeVisualAspectDependingOnFuel();

	// Function that was not in USceneComponent for some reason
	FVector GetWorldScale3D() const;

	// In liters
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float m_MaxFuelCapacity = 1.0f; // would be const if Uproperty allowed it

	// in L/km, can go up to 2 times this at full throttle
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float m_BaseFuelConsumptionRate = 1.0f; //would be const if Uproperty allowed it

	// in L/km
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float m_CurrentFuelConsumptionRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool m_Visible = true;

	// In liters
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float m_CurrentFuel; 

	UPROPERTY()
		UMaterialInstanceDynamic* m_pMaterialInstanceDynamic;
	UPROPERTY()
		UMaterial* m_pMaterial;

	const static int32 m_NumCurveSamples = 100; 
	float m_FuelConsumptionCurvePoints[m_NumCurveSamples];
	float m_BaseScaleY;


};
