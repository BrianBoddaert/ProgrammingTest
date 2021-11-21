// Fill out your copyright notice in the Description page of Project Settings.

#include "FuelHud.h"
#include "DigitalMindSoftPawn.h"
#include "FuelComponent.h"
#include "Engine/Canvas.h"
#include "Engine/Font.h"
#include "CanvasItem.h"
#include "RenderResource.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/Engine.h"

AFuelHud::AFuelHud()
	:m_FuelHudIconFlickerTimer{ 0.0f }
	, m_FuelHudIconFlickerActive{ false }
{
	if (!m_pLowFuelIndicatorTexture)
	{
		static ConstructorHelpers::FObjectFinder<UTexture2D> lowFuelImage(TEXT("/Game/Textures/LowfuelIndicator"));
		if (lowFuelImage.Succeeded())
			m_pLowFuelIndicatorTexture = lowFuelImage.Object;
	}
}

void AFuelHud::DrawHUD()
{
	Super::DrawHUD();

	const float HUDXRatio = Canvas->SizeX / 1280.f;
	const float HUDYRatio = Canvas->SizeY / 720.f;

	ADigitalMindSoftPawn* Vehicle = Cast<ADigitalMindSoftPawn>(GetOwningPawn());
	UFuelComponent* fuelComp = Cast<UFuelComponent>(Vehicle->GetComponentByClass(UFuelComponent::StaticClass()));

	if (!fuelComp)
		return;

	const float maxFuel = fuelComp->GetMaxFuel();
	const float currentFuel = fuelComp->GetCurrentFuel();
	const float lowfuelIndicatorActivationFraction = 0.3f;
	const float superLowfuelIndicatorActivationFraction = 0.15f;
	const float lowfuelindicatorScale = 0.05f;
	const float fuelFraction = currentFuel / maxFuel;

	if ((Vehicle != nullptr) && (Vehicle->bInCarCameraActive == false))
	{
		FVector2D ScaleVec(HUDYRatio * 1.4f, HUDYRatio * 1.4f);

#define LOCTEXT_NAMESPACE "FuelHUD"
		FText fuelText = FText::Format(LOCTEXT("FuelFormat", "{0} liters"), FText::AsNumber(currentFuel));
		FText consumptionText = FText::Format(LOCTEXT("ConsumptionFormat", "{0} l/km"), FText::AsNumber(fuelComp->GetCurrentConsumption()));
#undef LOCTEXT_NAMESPACE

		// Fuel
		FCanvasTextItem fuelTextItem(FVector2D(HUDXRatio * 805.f, HUDYRatio * 415), fuelText, HUDFont, FLinearColor::White);
		fuelTextItem.Scale = ScaleVec;
		Canvas->DrawItem(fuelTextItem);

		// Consumption rate
		FCanvasTextItem consumptionTextItem(FVector2D(HUDXRatio * 805.f, HUDYRatio * 370), consumptionText, HUDFont, FLinearColor::White);
		consumptionTextItem.Scale = ScaleVec;
		Canvas->DrawItem(consumptionTextItem);

		// Low fuel indicator
		if (fuelFraction < lowfuelIndicatorActivationFraction)
		{
			bool alarminglyLowFuel = fuelFraction < superLowfuelIndicatorActivationFraction;

			// Flickering of the low fuel icon
			m_FuelHudIconFlickerTimer += RenderDelta;
			if ((!alarminglyLowFuel && m_FuelHudIconFlickerTimer > m_LowFuelIconFlickerInterval) || 
				( alarminglyLowFuel && m_FuelHudIconFlickerTimer > m_AlarminglyLowFuelIconFlickerInterval))
			{
				m_FuelHudIconFlickerTimer = 0.0f;
				m_FuelHudIconFlickerActive = !m_FuelHudIconFlickerActive;
			}
			
			// Draw low fuel icon
			if (!m_FuelHudIconFlickerActive)
			{
				// Draw it red if fuel is alarmingly low
				if (alarminglyLowFuel)
					Canvas->SetLinearDrawColor(FLinearColor(1, 0, 0));
				else
					Canvas->SetLinearDrawColor(FLinearColor(1, 1, 1));

				Canvas->DrawTile(m_pLowFuelIndicatorTexture, HUDXRatio * 805.f, HUDYRatio * 315,
					m_pLowFuelIndicatorTexture->GetSurfaceWidth() * lowfuelindicatorScale * ScaleVec.X,
					m_pLowFuelIndicatorTexture->GetSurfaceHeight() * lowfuelindicatorScale * ScaleVec.Y,
					0, 0, m_pLowFuelIndicatorTexture->GetSurfaceWidth(), m_pLowFuelIndicatorTexture->GetSurfaceHeight(), EBlendMode::BLEND_Translucent);
			}

		}

	}

}