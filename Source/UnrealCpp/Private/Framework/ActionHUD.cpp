// Fill out your copyright notice in the Description page of Project Settings.

#include "Framework/ActionHUD.h"
#include "Blueprint/UserWidget.h"

UUserWidget* AActionHUD::GetMainHudWidget() const
{
	return MainHudWidgetInstance;
}

void AActionHUD::BeginPlay()
{
	Super::BeginPlay();

	if (MainHudWidgetClass)
	{
		MainHudWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), MainHudWidgetClass);
		if (MainHudWidgetInstance)
		{
			MainHudWidgetInstance->AddToViewport();
		}
	}
}