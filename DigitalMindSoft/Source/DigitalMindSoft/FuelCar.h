// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DigitalMindSoftPawn.h"
#include "FuelCar.generated.h"

class UFuelComponent;

UCLASS()
class DIGITALMINDSOFT_API AFuelCar : public ADigitalMindSoftPawn
{
	GENERATED_BODY()
public:
	AFuelCar();

	virtual void Tick(float Delta) override;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)



	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

	virtual void MoveForward(float val) override;
	void ToggleFuelComponentVisibility();

	UFUNCTION(BlueprintCallable)
		UFuelComponent* GetFuelComponent() const { return m_pFuelComponent;	}

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UFuelComponent* m_pFuelComponent = nullptr;

};
