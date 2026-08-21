// Fill out your copyright notice in the Description page of Project Settings.

#include "Data/Item/Action/ItemAction_Money.h"
#include "Interface/InterfaceInventoryUser.h"
#include "Component/InventoryComponent.h"

void UItemAction_Money::ExecuteAction(AActor* InInstigator, AActor* InTarget)
{
	if (IInterfaceInventoryUser* InvenUser = Cast<IInterfaceInventoryUser>(InTarget))
	{
		if (UInventoryComponent* InvenComp = InvenUser->GetInventoryComponent())
		{
			FInventoryCommandResult Result;
			InvenComp->ExecuteCommand(FInventoryCommand::MakeMoney(MoneyAmount), Result);
		}
	}
}
