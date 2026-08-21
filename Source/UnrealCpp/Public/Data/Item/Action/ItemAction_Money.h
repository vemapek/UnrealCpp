// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/Item/Action/ItemAction.h"
#include "ItemAction_Money.generated.h"

/**
 * 아이템 사용 시 돈을 지급하는 액션
 */
UCLASS()
class UNREALCPP_API UItemAction_Money : public UItemAction
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Money")
	int32 MoneyAmount = 100;

public:
	virtual void ExecuteAction(AActor* InInstigator, AActor* InTarget) override;
};
