// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUDWidget.generated.h"

class UStatBarWidget;
class UStatActorComponent;

/**
 * 체력바 + 스태미나바를 표시하는 플레이어 HUD (델리게이트 바인딩 방식)
 */
UCLASS()
class UNREALCPP_API UPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:
	// OnStaminaChange 델리게이트가 방송될 때마다 호출됨
	UFUNCTION()
	void HandleStaminaChange(float Current, float Max);

	// OnHealthChange 델리게이트가 방송될 때마다 호출됨
	UFUNCTION()
	void HandleHealthChange(float Current, float Max);

protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UStatBarWidget> HealthBar;

	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UStatBarWidget> StaminaBar;

private:
	// 델리게이트 해제(Remove)를 위해 참조를 들고 있음
	TWeakObjectPtr<UStatActorComponent> CachedStatComponent;
};