// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MoneyPanelWidget.generated.h"

class UTextBlock;

/**
 *
 */
UCLASS()
class UNREALCPP_API UMoneyPanelWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetMoney(int32 InMoney);

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> MoneyText;
};
