// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/DamagePopupWidget.h"
#include "Components/TextBlock.h"

void UDamagePopupWidget::SetDamage(float InDamage)
{
	DamageText->SetText(FText::AsNumber(static_cast<int32>(InDamage)));
}

void UDamagePopupWidget::PlayPopupAnimation()
{
	SetAnimationCurrentTime(PopupAnimation, 0.0f);	// 혹시 몰라 안전용으로
	PlayAnimation(PopupAnimation);
}

void UDamagePopupWidget::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
	Super::OnAnimationFinished_Implementation(Animation);

	if (Animation == PopupAnimation)
	{
		// 팝업 애니메이션이 종료됨
		OnPopUpAnimationFinished.ExecuteIfBound();
	}
}
