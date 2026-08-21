// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/DetailInfoWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/PanelWidget.h"
#include "Data/Item/ItemDataAsset.h"
#include "Blueprint/WidgetLayoutLibrary.h"

void UDetailInfoWidget::Open(const UItemDataAsset* InItemData)
{
	if (!InItemData) return;

	ItemIcon->SetBrushFromTexture(InItemData->Icon.Get());
	ItemNameText->SetText(InItemData->DisplayName);
	ItemPriceText->SetText(FText::AsNumber(InItemData->Price));
	ItemDescriptionText->SetText(InItemData->Description);

	SetVisibility(ESlateVisibility::HitTestInvisible);
	bTickEnable = true;

	UpdateLocation(); // 다음 틱까지 기다리지 않고 열리자마자 바로 마우스 위치로 이동
}

void UDetailInfoWidget::Close()
{
	SetVisibility(ESlateVisibility::Collapsed);
	bTickEnable = false;
}

void UDetailInfoWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CanvasSlot = Cast<UCanvasPanelSlot>(Slot);

	Close();
}

void UDetailInfoWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!bTickEnable) return;

	UpdateLocation();
}

void UDetailInfoWidget::UpdateLocation()
{
	if (!CanvasSlot.IsValid())
	{
		CanvasSlot = Cast<UCanvasPanelSlot>(Slot);
	}

	if (CanvasSlot.IsValid())
	{
		const FVector2D MouseAbsolute = UWidgetLayoutLibrary::GetMousePositionOnPlatform();
		if (UPanelWidget* ParentPanel = GetParent())
		{
			const FGeometry& ParentGeometry = ParentPanel->GetTickSpaceGeometry();
			const FVector2D LocalPos = ParentGeometry.AbsoluteToLocal(MouseAbsolute);
			CanvasSlot->SetPosition(LocalPos);
		}
	}
}
