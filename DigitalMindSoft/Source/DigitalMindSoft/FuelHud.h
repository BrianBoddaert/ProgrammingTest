// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DigitalMindSoftHud.h"
#include "FuelHud.generated.h"

/**
 * 
 */
UCLASS()
class DIGITALMINDSOFT_API AFuelHud : public ADigitalMindSoftHud
{
	GENERATED_BODY()

public:
	AFuelHud();
	virtual void DrawHUD() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTexture2D* m_pLowFuelIndicatorTexture = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float m_LowFuelIconFlickerInterval = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float m_AlarminglyLowFuelIconFlickerInterval = 0.5f;

	float m_FuelHudIconFlickerTimer;
	bool m_FuelHudIconFlickerActive;
};
