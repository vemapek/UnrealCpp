// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/InventorySlotWidget.h"
#include "Widget/InventoryDragDropOperation.h"
#include "Widget/TemporarySlotWidget.h"
#include "Component/InventoryComponent.h"
#include "Component/InventoryCommandTypes.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/HorizontalBox.h"

void UInventorySlotWidget::InitializeSlot(UInventoryComponent* InInven, int32 InIndex)
{
	if (!InInven) return;

	TargetInventory = InInven;
	Index = InIndex;

	RefreshSlot();
}

void UInventorySlotWidget::RefreshSlot() const
{
	// 슬롯 포인터를 캐싱해두면 나중에 배열 갱신 등으로 끊어질 수 있어서, 매번 새로 받아온다.
	if (!TargetInventory.IsValid()) return;

	const FInvenSlot* InvenSlot = TargetInventory->GetSlot(Index); // UWidget에 이미 Slot 멤버가 있어서 이름 충돌 방지
	if (!InvenSlot)
	{
		UE_LOG(LogTemp, Warning, TEXT("[Slot %d]가 null입니다."), Index);
		return;
	}

	if (InvenSlot->IsEmpty())
	{
		// 슬롯이 비어있으면
		IconImage->SetBrushFromTexture(nullptr);
		IconImage->SetBrushTintColor(FLinearColor::Transparent);
		CountBox->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		// 슬롯이 비어있지 않으면
		IconImage->SetBrushFromTexture(InvenSlot->ItemData->Icon.Get());
		IconImage->SetBrushTintColor(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));

		CountText->SetText(FText::AsNumber(InvenSlot->GetCount()));
		MaxStackText->SetText(FText::AsNumber(InvenSlot->ItemData->MaxStackCount));
		CountBox->SetVisibility(ESlateVisibility::HitTestInvisible);
	}

}

void UInventorySlotWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	OnSlotEnter.Broadcast(Index);
}

void UInventorySlotWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	OnSlotLeave.Broadcast();
	Super::NativeOnMouseLeave(InMouseEvent);
}

FReply UInventorySlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (TargetInventory.IsValid())
	{
		if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
		{
			if (FInvenSlot* InvenSlot = TargetInventory->GetSlot(Index))
			{
				if (!InvenSlot->IsEmpty())
				{
					return FReply::Handled().DetectDrag(TakeWidget(), EKeys::LeftMouseButton);
				}
			}
		}
		else if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
		{
			// 우클릭으로 아이템 사용
			if (FInvenSlot* InvenSlot = TargetInventory->GetSlot(Index))
			{
				if (!InvenSlot->IsEmpty())
				{
					FInventoryCommand Command = FInventoryCommand::MakeUse(Index);
					FInventoryCommandResult Result;
					TargetInventory->ExecuteCommand(Command, Result);
				}
			}
		}
	}
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UInventorySlotWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	if (!TargetInventory.IsValid()) return;

	FInvenSlot* InvenSlot = TargetInventory->GetSlot(Index);
	if (!InvenSlot || InvenSlot->IsEmpty()) return;

	UInventoryDragDropOperation* DragOp = NewObject<UInventoryDragDropOperation>();
	DragOp->SourceIndex = Index;
	DragOp->ItemData = InvenSlot->ItemData;
	DragOp->Count = InvenSlot->GetCount();
	DragOp->Pivot = EDragPivot::MouseDown;

	if (TSubclassOf<UTemporarySlotWidget> TempClass = TargetInventory->GetTemporarySlotWidgetClass())
	{
		if (UTemporarySlotWidget* DragVisual = CreateWidget<UTemporarySlotWidget>(this, TempClass))
		{
			DragVisual->SetItem(InvenSlot->ItemData, InvenSlot->GetCount());
			DragOp->DefaultDragVisual = DragVisual;
		}
	}

	OutOperation = DragOp;	// NativeOnDrop을 발동시키기 위해 필수
}

bool UInventorySlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	if (UInventoryDragDropOperation* DragOp = Cast<UInventoryDragDropOperation>(InOperation))
	{
		if (TargetInventory.IsValid() && DragOp->SourceIndex != Index)
		{
			FInventoryCommand Command = FInventoryCommand::MakeMove(DragOp->SourceIndex, Index);
			FInventoryCommandResult Result;
			TargetInventory->ExecuteCommand(Command, Result);
		}
	}
	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}

void UInventorySlotWidget::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragCancelled(InDragDropEvent, InOperation);

	// 인벤토리 슬롯이 아닌 곳(게임 화면)에 드랍하면 버려진다. 실제 위치는
	// InventoryComponent::HandleDropCommand가 항상 대상(플레이어) 앞쪽으로 던지는 걸로
	// 알아서 계산하므로, 여기서는 바닥 위치를 따로 계산할 필요가 없다.
	UInventoryDragDropOperation* DragOp = Cast<UInventoryDragDropOperation>(InOperation);
	if (!DragOp || !TargetInventory.IsValid()) return;

	FInventoryCommand Command = FInventoryCommand::MakeDrop(DragOp->SourceIndex, FVector::ZeroVector);
	FInventoryCommandResult Result;
	TargetInventory->ExecuteCommand(Command, Result);
}
