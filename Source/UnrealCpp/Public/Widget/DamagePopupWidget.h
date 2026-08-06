// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DamagePopupWidget.generated.h"

class UTextBlock;

DECLARE_DELEGATE(FOnPopUpAnimationFinished);

/**
 *
 */
UCLASS()
class UNREALCPP_API UDamagePopupWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetDamage(float InDamage);

	UFUNCTION(BlueprintCallable)
	void PlayPopupAnimation();

protected:
	virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;

public:
	FOnPopUpAnimationFinished OnPopUpAnimationFinished;

protected:
	UPROPERTY(VisibleDefaultsOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> DamageText;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> PopupAnimation;
};
