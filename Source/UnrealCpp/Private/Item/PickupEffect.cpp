// Fill out your copyright notice in the Description page of Project Settings.

#include "Item/PickupEffect.h"
#include "Component/StatActorComponent.h"
#include "Interface/InterfaceStamina.h"
#include "Interface/InterfaceHealth.h"
#include "Interface/InterfaceStat.h"

void APickupEffect::OnPickup(AActor* InTarget)
{
	Super::OnPickup(InTarget);

	// Cast<IInterfaceStat>가 성공하면 = 이 액터가 스탯 컴포넌트를 가지고 있다는 뜻
	if (IInterfaceStat* Stat = Cast<IInterfaceStat>(InTarget))
	{
		UStatActorComponent* StatComp = Stat->GetStatComponent();

		if (Stamina > 0)
		{
			IInterfaceStamina::Execute_RecoveryStamina(StatComp, Stamina);
		}
		else if (Stamina < 0)
		{
			IInterfaceStamina::Execute_ConsumeStamina(StatComp, -Stamina);
		}

		if (Health > 0)
		{
			IInterfaceHealth::Execute_HealHealth(StatComp, Health);
		}
		else if (Health < 0)
		{
			IInterfaceHealth::Execute_DamageHealth(StatComp, -Health);
		}
	}
}