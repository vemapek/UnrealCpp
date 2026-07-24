// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerHUDWidget.h"
#include "StatBarWidget.h"
#include "StatActorComponent.h"
#include "InterfaceStat.h"
#include "InterfaceStamina.h"
#include "InterfaceHealth.h"

void UPlayerHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 이 HUD를 소유한 플레이어의 폰(캐릭터)을 가져와서
	if (APawn* OwningPawn = GetOwningPlayerPawn())
	{
		// 그 폰이 IInterfaceStat을 구현했는지 확인하고, 구현했으면 StatComponent를 받아옴
		if (IInterfaceStat* Stat = Cast<IInterfaceStat>(OwningPawn))
		{
			if (UStatActorComponent* StatComp = Stat->GetStatComponent())
			{
				CachedStatComponent = StatComp;

				// 값이 바뀔 때만 알림받도록 델리게이트에 함수 등록
				StatComp->OnStaminaChange.AddDynamic(this, &UPlayerHUDWidget::HandleStaminaChange);
				StatComp->OnHealthChange.AddDynamic(this, &UPlayerHUDWidget::HandleHealthChange);

				// 델리게이트는 "변화가 생겼을 때"만 호출되니까,
				// 위젯이 처음 뜬 시점의 현재값으로 한 번은 직접 갱신해줘야 함
				const float CurrentStamina = IInterfaceStamina::Execute_GetCurrentStamina(StatComp);
				const float MaxStamina = IInterfaceStamina::Execute_GetMaxStamina(StatComp);
				HandleStaminaChange(CurrentStamina, MaxStamina);

				const float CurrentHealth = IInterfaceHealth::Execute_GetCurrentHealth(StatComp);
				const float MaxHealth = IInterfaceHealth::Execute_GetMaxHealth(StatComp);
				HandleHealthChange(CurrentHealth, MaxHealth);
			}
		}
	}
}

void UPlayerHUDWidget::NativeDestruct()
{
	// 위젯이 사라질 때 등록해둔 델리게이트를 반드시 해제 (안 하면 위젯이 사라져도 계속 호출 시도해서 위험함)
	if (CachedStatComponent.IsValid())
	{
		CachedStatComponent->OnStaminaChange.RemoveDynamic(this, &UPlayerHUDWidget::HandleStaminaChange);
		CachedStatComponent->OnHealthChange.RemoveDynamic(this, &UPlayerHUDWidget::HandleHealthChange);
	}

	Super::NativeDestruct();
}

void UPlayerHUDWidget::HandleStaminaChange(float Current, float Max)
{
	if (StaminaBar)
	{
		StaminaBar->UpdateStat(Current, Max);
	}
}

void UPlayerHUDWidget::HandleHealthChange(float Current, float Max)
{
	if (HealthBar)
	{
		HealthBar->UpdateStat(Current, Max);
	}
}

