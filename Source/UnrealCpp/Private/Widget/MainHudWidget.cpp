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
