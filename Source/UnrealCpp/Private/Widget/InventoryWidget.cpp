// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/InventoryWidget.h"
#include "Widget/MoneyPanelWidget.h"
#include "Widget/InventorySlotWidget.h"
#include "Components/Button.h"
#include "Components/UniformGridPanel.h"
#include "Component/InventoryComponent.h"
#include "Interface/InterfaceInventoryUser.h"
#include "GameFramework/PlayerController.h"


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
	SetVisibility(ESlateVisibility::Visible);
	bIsOpen = true;

	if (APlayerController* PC = GetOwningPlayer())
	{
		FInputModeGameAndUI InputMode;
		InputMode.SetWidgetToFocus(TakeWidget());
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		PC->SetInputMode(InputMode);
		PC->SetShowMouseCursor(true);
	}
}

void UInventoryWidget::CloseInventoryWidget()
{
	SetVisibility(ESlateVisibility::Collapsed);
	bIsOpen = false;

	if (APlayerController* PC = GetOwningPlayer())
	{
		FInputModeGameOnly InputMode;
		PC->SetInputMode(InputMode);
		PC->SetShowMouseCursor(false);
	}
}

void UInventoryWidget::ToggleInventoryWidget()
{
	UE_LOG(LogTemp, Warning, TEXT("[Toggle] 7. InventoryWidget::ToggleInventoryWidget 실행됨. 현재 bIsOpen = %s"),
		bIsOpen ? TEXT("true") : TEXT("false"));

	if (bIsOpen)
	{
		CloseInventoryWidget();
	}
	else
	{
		OpenInventoryWidget();
	}

	UE_LOG(LogTemp, Warning, TEXT("[Toggle] 8. 토글 후 bIsOpen = %s, Visibility = %d"),
		bIsOpen ? TEXT("true") : TEXT("false"), (int32)GetVisibility());
}

bool UInventoryWidget::IsInventoryOpen() const
{
	return bIsOpen;
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

	// HUD에 항상 붙어있는 위젯이라, 시작할 때는 닫혀있는 상태로 시작
	SetVisibility(ESlateVisibility::Collapsed);
	bIsOpen = false;

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
