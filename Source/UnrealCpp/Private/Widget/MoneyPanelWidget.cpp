// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/MoneyPanelWidget.h"
#include "Components/TextBlock.h"

void UMoneyPanelWidget::SetMoney(int32 InMoney)
{
	MoneyText->SetText(FText::AsNumber(InMoney));
}
