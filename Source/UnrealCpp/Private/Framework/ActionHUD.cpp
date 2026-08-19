// Fill out your copyright notice in the Description page of Project Settings.

#include "Framework/ActionHUD.h"
#include "Widget/MainHudWidget.h"

UMainHudWidget* AActionHUD::GetMainHudWidget() const
{
	return MainHudWidgetInstance;
}

void AActionHUD::BeginPlay()
{
	Super::BeginPlay();

	if (MainHudWidgetClass)
	{
		MainHudWidgetInstance = CreateWidget<UMainHudWidget>(GetWorld(), MainHudWidgetClass);
		if (MainHudWidgetInstance)
		{
			MainHudWidgetInstance->AddToViewport();
		}
	}
}
