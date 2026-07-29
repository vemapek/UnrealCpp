// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUDWidget.generated.h"

class UStatBarWidget;

/**
 * 체력바 + 스태미나바를 표시하는 플레이어 HUD
 */
UCLASS()
class UNREALCPP_API UPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

private:
	void InitializePlayerStatBars();

protected:
	// 위젯 블루프린트 안에 "HealthBar"라는 이름으로 StatBarWidget 인스턴스를 배치해야 함
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UStatBarWidget> HealthBar;

	// 위젯 블루프린트 안에 "StaminaBar"라는 이름으로 StatBarWidget 인스턴스를 배치해야 함
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UStatBarWidget> StaminaBar;
};