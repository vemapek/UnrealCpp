// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainHudWidget.generated.h"

class UPlayerHUDWidget;
class UInventoryWidget;

/**
 *
 */
UCLASS()
class UNREALCPP_API UMainHudWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void TestInventoryRefresh() const;

	// 인벤토리 위젯을 열려있으면 닫고, 닫혀있으면 여는 함수(E키 등 입력에서 호출)
	UFUNCTION(BlueprintCallable)
	void ToggleInventory() const;

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UPlayerHUDWidget> PlayerHUD;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UInventoryWidget> Inventory;
};
