// Fill out your copyright notice in the Description page of Project Settings.

#include "StatBarWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UStatBarWidget::UpdateStat(float CurrentValue, float MaxValue)
{
	if (Bar)
	{
		const float Percent = (MaxValue > 0.0f) ? (CurrentValue / MaxValue) : 0.0f;
		Bar->SetPercent(FMath::Clamp(Percent, 0.0f, 1.0f));
	}

	if (ValueText)
	{
		const FText DisplayText = FText::FromString(FString::Printf(TEXT("%.0f / %.0f"), CurrentValue, MaxValue));
		ValueText->SetText(DisplayText);
	}
}