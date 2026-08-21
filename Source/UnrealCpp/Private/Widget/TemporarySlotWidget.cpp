// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/TemporarySlotWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Data/Item/ItemDataAsset.h"

void UTemporarySlotWidget::SetItem(const UItemDataAsset* InItemData, int32 InCount)
{
	if (!InItemData) return;

	if (IconImage)
	{
		IconImage->SetBrushFromTexture(InItemData->Icon.Get());
	}

	if (CountText)
	{
		if (InCount > 1)
		{
			CountText->SetText(FText::AsNumber(InCount));
			CountText->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
		else
		{
			CountText->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}
