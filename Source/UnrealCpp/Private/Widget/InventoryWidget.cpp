// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/InventoryWidget.h"
#include "Widget/MoneyPanelWidget.h"
#include "Widget/InventorySlotWidget.h"
#include "Widget/DetailInfoWidget.h"
#include "Widget/InventoryDragDropOperation.h"
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

				SlotWidget->OnSlotEnter.AddWeakLambda(
					this,
					[this](int32 InIndex)
					{
						if (TargetInventory.IsValid() && DetailInfo)
						{
							if (FInvenSlot* InvenSlot = TargetInventory->GetSlot(InIndex))
							{
								DetailInfo->Open(InvenSlot->ItemData);
							}
						}
					}
				);
				SlotWidget->OnSlotLeave.AddWeakLambda(
					this,
					[this]()
					{
						if (DetailInfo)
						{
							DetailInfo->Close();
						}
					}
				);

				SlotWidgets.Add(SlotWidget);
			}
		}
	}
	RefreshInventoryWidget();
}

void UInventoryWidget::ClearInventoryWidget()
{
	SlotWidgets.Empty(); // 재초기화 시 슬롯 위젯이 중복으로 쌓이는 것 방지
	if (TargetInventory.IsValid())
	{
		TargetInventory->OnSlotChanged.Unbind();
		TargetInventory->OnMoneyChanged.RemoveAll(this); // Clear()는 다른 리스너까지 다 지워버림
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

bool UInventoryWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	// 인벤토리 위젯의 루트 캔버스는 보통 화면 전체를 덮기 때문에, 여기서 무조건 다 잡아버리면
	// 게임 화면(3D 월드)에 드랍해도 전부 "인벤토리 안"으로 처리되어 바닥에 버리는 기능이 죽어버린다.
	// 그래서 실제 슬롯 그리드 영역 안에 드랍됐을 때만 여기서 처리(제자리로 되돌림)하고,
	// 그 밖은 처리하지 않고 그대로 흘려보내서 NativeOnDragCancelled가 정상 실행되게 한다.
	// (원본 슬롯의 데이터를 드래그 중에 건드리지 않는 구조라, 여기서 처리됐다고만 알려주면
	// 아이템은 자동으로 원래 자리에 그대로 남는다)
	if (Cast<UInventoryDragDropOperation>(InOperation) && SlotGridPanel)
	{
		const FVector2D AbsolutePosition = InDragDropEvent.GetScreenSpacePosition();
		if (SlotGridPanel->GetTickSpaceGeometry().IsUnderLocation(AbsolutePosition))
		{
			return true;
		}
	}
	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}

void UInventoryWidget::OnClickedCloseButton()
{
	UE_LOG(LogTemp, Log, TEXT("닫기 버튼 눌려짐"));
	CloseInventoryWidget();
}
