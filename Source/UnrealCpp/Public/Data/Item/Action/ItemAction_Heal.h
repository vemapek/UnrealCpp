// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/Item/Action/ItemAction.h"
#include "ItemAction_Heal.generated.h"

/**
 * 아이템 사용 시 체력을 회복시키는 액션
 */
UCLASS()
class UNREALCPP_API UItemAction_Heal : public UItemAction
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Heal")
	float HealAmount = 30.0f;

public:
	virtual void ExecuteAction(AActor* InInstigator, AActor* InTarget) override;
};
