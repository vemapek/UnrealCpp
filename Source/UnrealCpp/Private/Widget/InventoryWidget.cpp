// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/InventoryWidget.h"
#include "Widget/MoneyPanelWidget.h"
#include "Widget/InventorySlotWidget.h"
#include "Components/Button.h"
#include "Components/UniformGridPanel.h"
#include "Component/InventoryComponent.h"
#include "Interface/InterfaceInventoryUser.h"


void UInventoryWidget::InitializeInventoryWidget(UInventoryComponent* InInven)
{
	ClearInventoryWidget();
	TargetInventory = InInven;

	if (!TargetInventory.IsValid())
	{
		UE_LOG(LogTemp, Log, TEXT("TargetInventory가 Valid하지 않습니다."));
		return;
	}

	TargetInventory->OnSlotChanged.BindUObject(this, &UInventoryWidget::RefreshSlotWidget);
	TargetInventory->OnMoneyChanged.AddUObject(this, &UInventoryWidget::RefreshMoneyPanel);

	if (SlotGridPanel)
	{
		int32 ChildCount = SlotGridPanel->GetChildrenCount();
		int32 InvenSize = TargetInventory->GetSize();
		SlotSize = FMath::Min(ChildCount, InvenSize);

		SlotWidgets.Empty(SlotSize);
		for (int i = 0; i < SlotSize; i++)
		{
			if (UInventorySlotWidget* SlotWidget = Cast<UInventorySlotWidget>(SlotGridPanel->GetChildAt(i)))
			{
				SlotWidget->InitializeSlot(TargetInventory.Get(), i);
				SlotWidgets.Add(SlotWidget);
			}
		}
	}
	RefreshInventoryWidget();
}

void UInventoryWidget::ClearInventoryWidget()
{
	if (TargetInventory.IsValid())
	{
		TargetInventory->OnSlotChanged.Unbind();
		TargetInventory->OnMoneyChanged.Clear();
		TargetInventory = nullptr;
	}
	SlotSize = 0;
}

void UInventoryWidget::OpenInventoryWidget()
{
}

void UInventoryWidget::CloseInventoryWidget()
{
}

void UInventoryWidget::TestRefresh()
{
	RefreshInventoryWidget();
}

void UInventoryWidget::RefreshInventoryWidget() const
{
	if (!TargetInventory.IsValid()) return;

	RefreshMoneyPanel(TargetInventory->GetMoney());

	for (const UInventorySlotWidget* SlotWidget : SlotWidgets)
	{
		if (SlotWidget)
		{
			SlotWidget->RefreshSlot();
		}
	}
}

void UInventoryWidget::RefreshSlotWidget(int32 InSlotIndex) const
{
	if (IsValidIndex(InSlotIndex) && SlotWidgets[InSlotIndex])
	{
		SlotWidgets[InSlotIndex]->RefreshSlot();
	}
}

void UInventoryWidget::RefreshMoneyPanel(int32 InCurrentMoney) const
{
	if (MoneyPanel)
	{
		MoneyPanel->SetMoney(InCurrentMoney);
	}
}

void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (CloseButton)
	{
		CloseButton->OnClicked.AddDynamic(this, &UInventoryWidget::OnClickedCloseButton);
	}

	if (IInterfaceInventoryUser* InvenUser = Cast<IInterfaceInventoryUser>(GetOwningPlayerPawn()))
	{
		if (UInventoryComponent* InvenComp = InvenUser->GetInventoryComponent())
		{
			InitializeInventoryWidget(InvenComp);
		}
	}
}

void UInventoryWidget::OnClickedCloseButton()
{
	UE_LOG(LogTemp, Log, TEXT("닫기 버튼 눌려짐"));
	CloseInventoryWidget();
}
