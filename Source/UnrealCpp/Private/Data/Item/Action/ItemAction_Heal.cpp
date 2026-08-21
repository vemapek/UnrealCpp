// Fill out your copyright notice in the Description page of Project Settings.

#include "Data/Item/Action/ItemAction_Heal.h"
#include "Interface/InterfaceStat.h"
#include "Interface/InterfaceHealth.h"
#include "Component/StatActorComponent.h"

void UItemAction_Heal::ExecuteAction(AActor* InInstigator, AActor* InTarget)
{
	if (IInterfaceStat* StatUser = Cast<IInterfaceStat>(InTarget))
	{
		if (UStatActorComponent* StatComp = StatUser->GetStatComponent())
		{
			IInterfaceHealth::Execute_HealHealth(StatComp, HealAmount);
		}
	}
}
