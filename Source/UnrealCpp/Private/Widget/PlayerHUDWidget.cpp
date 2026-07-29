// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/PlayerHUDWidget.h"
#include "Widget/StatBarWidget.h"
#include "Interface/InterfaceStat.h"
#include "Interface/InterfaceHealth.h"
#include "Interface/InterfaceStamina.h"
#include "Component/StatActorComponent.h"

void UPlayerHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
	InitializePlayerStatBars();
}

void UPlayerHUDWidget::InitializePlayerStatBars()
{
	if (IInterfaceStat* Player = Cast<IInterfaceStat>(GetOwningPlayerPawn()))
	{
		if (UStatActorComponent* Stat = Player->GetStatComponent())
		{
			// 각 바(HealthBar, StaminaBar)를 컴포넌트 델리게이트에 직접 등록
			Stat->OnHealthChange.AddDynamic(HealthBar, &UStatBarWidget::UpdateStat);
			Stat->OnStaminaChange.AddDynamic(StaminaBar, &UStatBarWidget::UpdateStat);

			// 델리게이트는 "변화가 생겼을 때"만 호출되니까, 처음 뜬 시점 값으로 한 번은 직접 갱신
			HealthBar->UpdateStat(
				IInterfaceHealth::Execute_GetCurrentHealth(Stat),
				IInterfaceHealth::Execute_GetMaxHealth(Stat));
			StaminaBar->UpdateStat(
				IInterfaceStamina::Execute_GetCurrentStamina(Stat),
				IInterfaceStamina::Execute_GetMaxStamina(Stat));
		}
	}
}