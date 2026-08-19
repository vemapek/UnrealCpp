// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/InventorySlotWidget.h"
#include "Component/InventoryComponent.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/HorizontalBox.h"

void UInventorySlotWidget::InitializeSlot(UInventoryComponent* InInven, int32 InIndex)
{
	if (!InInven) return;

	TargetInventory = InInven;
	Index = InIndex;

	Slot = TargetInventory->GetSlot(Index);
	RefreshSlot();
}

void UInventorySlotWidget::RefreshSlot() const
{
	if (!Slot)
	{
		UE_LOG(LogTemp, Warning, TEXT("[Slot %d]가 null입니다."), Index);
		return;
	}

	if (Slot->IsEmpty())
	{
		// 슬롯이 비어있으면
		IconImage->SetBrushFromTexture(nullptr);
		IconImage->SetBrushTintColor(FLinearColor::Transparent);
		CountBox->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		// 슬롯이 비어있지 않으면
		IconImage->SetBrushFromTexture(Slot->ItemData->Icon.Get());
		IconImage->SetBrushTintColor(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));

		CountText->SetText(FText::AsNumber(Slot->GetCount()));
		MaxStackText->SetText(FText::AsNumber(Slot->ItemData->MaxStackCount));
		CountBox->SetVisibility(ESlateVisibility::HitTestInvisible);
	}

}

void UInventorySlotWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	UE_LOG(LogTemp, Log, TEXT("OnMouseEnter : %d 슬롯"), Index);
}

void UInventorySlotWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	UE_LOG(LogTemp, Log, TEXT("OnMouseLeave : %d 슬롯"), Index);
	Super::NativeOnMouseLeave(InMouseEvent);
}
