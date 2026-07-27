// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StatBarWidget.generated.h"

class UProgressBar;
class UTextBlock;

/**
 * 체력/스태미나 등 "현재값 / 최댓값" 형태의 스탯을 게이지 바로 보여주는 재사용 위젯
 */
UCLASS()
class UNREALCPP_API UStatBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// 현재값/최댓값을 받아서 게이지 채움 비율과 텍스트를 갱신
	UFUNCTION(BlueprintCallable)
	void UpdateStat(float CurrentValue, float MaxValue);

protected:
	// 채워지는 게이지 바 (에디터에서 이름을 "Bar"로 맞춰야 함)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UProgressBar> Bar;

	// "100 / 100" 같은 수치 표시 (에디터에서 이름을 "ValueText"로 맞춰야 함)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> ValueText;
};