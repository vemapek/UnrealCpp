// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/MainHudWidget.h"
#include "Widget/InventoryWidget.h"

void UMainHudWidget::TestInventoryRefresh() const
{
#if WITH_EDITOR
	if (Inventory)
	{
		Inventory->TestRefresh();
	}
#endif
}

void UMainHudWidget::ToggleInventory() const
{
	UE_LOG(LogTemp, Warning, TEXT("[Toggle] 6. MainHudWidget::ToggleInventory 실행됨. Inventory = %s"),
		Inventory ? TEXT("Valid") : TEXT("nullptr(BindWidget 이름 확인 필요)"));

	if (Inventory)
	{
		Inventory->ToggleInventoryWidget();
	}
}
